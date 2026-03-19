# Arduino UNO R3 分部分控制 + 软件建模完整包

## 1. 硬件接线
- D3  -> Base servo
- D5  -> Shoulder servo
- D6  -> Elbow servo
- D9  -> Wrist servo
- D10 -> Gripper servo
- D2  -> Gripper switch
- D4  -> Fixture sensor
- D7  -> E-stop

## 2. 非常重要
舵机不要直接吃 UNO 5V。必须外部 5V 电源供舵机，并与 UNO 共地。

## 3. Windows 先测串口
```bash
python windows_tools/serial_cli.py
```
或
```bash
python windows_tools/preset_sequence_test.py
```

## 4. 复制 ROS2 包到工作区
```bash
mkdir -p ~/ros2_ws/src
cp -r /mnt/c/Users/lchgjj/Desktop/tentacle_partial_control_full_package/ros2_ws/src/* ~/ros2_ws/src/
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
colcon build
source ~/ros2_ws/install/setup.bash
```

## 5. RViz 显示
```bash
ros2 launch tentacle_gazebo display.launch.py
```

## 6. Gazebo 显示
```bash
ros2 launch tentacle_gazebo gazebo_sim.launch.py
```

## 7. ROS2 串口桥
```bash
ros2 run tentacle_control serial_bridge --ros-args -p port:=COM3 -p baud:=115200
```

## 8. ROS2 终端控制
```bash
ros2 run tentacle_control cli_commander
```
