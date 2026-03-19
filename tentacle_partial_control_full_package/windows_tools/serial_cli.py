import serial
import time

PORT = "COM3"
BAUD = 115200

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

def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(2)
    print("串口已连接:", PORT)
    print(HELP)
    while True:
        cmd = input("请输入命令> ").strip()
        if not cmd:
            continue
        if cmd.lower() == "quit":
            break
        ser.write((cmd + "\n").encode())
        time.sleep(0.2)
        t0 = time.time()
        while time.time() - t0 < 1.5:
            line = ser.readline().decode(errors="ignore").strip()
            if line:
                print("UNO:", line)
    ser.close()

if __name__ == "__main__":
    main()
