# miniROSCar
Implementation of ROS to receive and process information from several sensors and send orders to actuators.

To communicate the control messages it uses a geometry_msgs::Twist format as the following:

## Build the node:
This node was made for [ROS Kinetic](ROS Kinetic), and tested in an environment using gcc (Ubuntu 5.4.0-6ubuntu1~16.04.9).

In order to build the executable node the next steps are suggested:

1. Make the root folder _miniROSCar_ a catkin workspace,
   - Go to the _miniROSCar_ and execute `catkin_make`
2. Wait until the building process is finished and execute `source devel/setup.bash` now the node is ready to run.

## Run the node
The standard notation package and node is enough to run the node...
1. `roscore`
2. `rosrun kontrollieren kontrol_trajectory_node`

##How to use it

The node subscribes to three signals from different sensors, thanks to the wrapping facilities of ROS it's just a matter of asking to the correct topics. Wich in this case will be three:

- "/turtle1/cmd_vel" (geometry_msgs::Twist robot_position)
  - This one is special, it's a node from the turtle sim package which listens to a given input on the keyboard.
- "/ultrasound" (sensor_msgs::Range robot_positionRange)
- "/imu" (sensor_msgs::Imu robot_positionImu)

The node publishes the control signals (which go direct into another node inside an arduino to control two actuators in a little car).

- geometry_msgs::Twist command_msg (geometry_msgs::Twist in the topic "/kontrolliert")


### Additional notes:
The format of the control signal is the following:

```
For geometry_msgs::Twist using:
 		dummy.linear.x
 		dummy.linear.y
 		dummy.angular.z

```
The node frome turtlesim needed is:

```
rossun turtlesim turtle_teleop_key
```
To change the "mode of operation" of the car (from manual control to following and object) is:

```
rostopic pub /turtle1/cmd_vel geometry_msgs/Twist '[0.0, 1.0, 0.0]' '[0.0, 0.0, 0.0]'

```
