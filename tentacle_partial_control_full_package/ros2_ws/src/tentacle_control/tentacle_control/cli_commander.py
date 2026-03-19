import rclpy
from rclpy.node import Node
from std_msgs.msg import String

HELP = """
命令:
HOME
POSE:TRAY_APPROACH
POSE:TRAY_PICK
POSE:TRAY_LIFT
POSE:FIXTURE_APPROACH
POSE:FIXTURE_INSERT
POSE:FIXTURE_PULL
POSE:RECOVERY_APPROACH
POSE:RECOVERY_RELEASE
OPEN_GRIPPER
CLOSE_GRIPPER
STATUS
DEMO
SET:J1:90
SET:J2:80
SET:J3:100
SET:J4:70
SET:G:40
quit
"""

class CLICommander(Node):
    def __init__(self):
        super().__init__('tentacle_cli_commander')
        self.pub = self.create_publisher(String, '/tentacle_command', 10)

    def send(self, text):
        msg = String()
        msg.data = text
        self.pub.publish(msg)
        self.get_logger().info(f'Published: {text}')

def main():
    rclpy.init()
    node = CLICommander()
    print(HELP)
    try:
        while rclpy.ok():
            cmd = input('请输入命令> ').strip()
            if not cmd:
                continue
            if cmd.lower() == 'quit':
                break
            node.send(cmd)
            rclpy.spin_once(node, timeout_sec=0.1)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
