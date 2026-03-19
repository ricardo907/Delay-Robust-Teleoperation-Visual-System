from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    xacro_file = PathJoinSubstitution([FindPackageShare('tentacle_description'), 'urdf', 'tentacle_robot.urdf.xacro'])
    world_file = PathJoinSubstitution([FindPackageShare('tentacle_gazebo'), 'worlds', 'tentacle_world.world'])
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([PathJoinSubstitution([FindPackageShare('gazebo_ros'), 'launch', 'gazebo.launch.py'])]),
        launch_arguments={'world': world_file}.items()
    )
    robot_description = {'robot_description': Command(['xacro ', xacro_file])}
    return LaunchDescription([
        gazebo_launch,
        Node(package='joint_state_publisher_gui', executable='joint_state_publisher_gui', output='screen'),
        Node(package='robot_state_publisher', executable='robot_state_publisher', parameters=[robot_description], output='screen'),
        Node(package='gazebo_ros', executable='spawn_entity.py', arguments=['-topic', 'robot_description', '-entity', 'tentacle_robot'], output='screen'),
    ])
