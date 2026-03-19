from setuptools import setup

package_name = 'tentacle_control'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools', 'pyserial'],
    zip_safe=True,
    maintainer='user',
    maintainer_email='user@example.com',
    description='ROS2 bridge and CLI for UNO-based tentacle arm.',
    license='MIT',
    entry_points={
        'console_scripts': [
            'serial_bridge = tentacle_control.serial_bridge:main',
            'cli_commander = tentacle_control.cli_commander:main',
        ],
    },
)
