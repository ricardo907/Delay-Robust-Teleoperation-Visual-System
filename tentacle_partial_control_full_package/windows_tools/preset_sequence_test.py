import serial
import time

PORT = "COM3"
BAUD = 115200

sequence = [
    "HOME",
    "POSE:TRAY_APPROACH",
    "POSE:TRAY_PICK",
    "CLOSE_GRIPPER",
    "POSE:TRAY_LIFT",
    "POSE:FIXTURE_APPROACH",
    "POSE:FIXTURE_INSERT",
    "POSE:FIXTURE_PULL",
    "POSE:RECOVERY_APPROACH",
    "POSE:RECOVERY_RELEASE",
    "OPEN_GRIPPER",
    "HOME"
]

def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(2)
    for cmd in sequence:
        print("SEND:", cmd)
        ser.write((cmd + "\n").encode())
        time.sleep(1.0)
        while ser.in_waiting:
            print("UNO:", ser.readline().decode(errors="ignore").strip())
    ser.close()

if __name__ == "__main__":
    main()
