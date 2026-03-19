import serial
import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class SerialBridge(Node):
    def __init__(self):
        super().__init__('tentacle_serial_bridge')
        self.declare_parameter('port', 'COM3')
        self.declare_parameter('baud', 115200)
        port = self.get_parameter('port').get_parameter_value().string_value
        baud = self.get_parameter('baud').get_parameter_value().integer_value
        self.get_logger().info(f'Opening serial port: {port} @ {baud}')
        self.ser = serial.Serial(port, baud, timeout=0.1)
        self.cmd_sub = self.create_subscription(String, '/tentacle_command', self.cmd_callback, 10)
        self.status_pub = self.create_publisher(String, '/tentacle_status', 10)
        self.timer = self.create_timer(0.05, self.read_serial)

    def cmd_callback(self, msg: String):
        self.ser.write((msg.data.strip() + '\n').encode())
        self.get_logger().info(f'SENT -> {msg.data}')

    def read_serial(self):
        if self.ser.in_waiting:
            line = self.ser.readline().decode(errors='ignore').strip()
            if line:
                msg = String()
                msg.data = line
                self.status_pub.publish(msg)
                self.get_logger().info(f'RECV <- {line}')

def main():
    rclpy.init()
    node = SerialBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        try:
            node.ser.close()
        except Exception:
            pass
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
