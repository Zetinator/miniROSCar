#include <ros/ros.h>
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <turtlesim/Pose.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>

using namespace std;

//geometry_msgs::Twist using:
// 		dummy.linear.x
// 		dummy.linear.y
// 		dummy.angular.z


// Some global variables
sensor_msgs::Range robot_positionRange;
sensor_msgs::Imu robot_positionImu;

geometry_msgs::Twist robot_position;
geometry_msgs::Twist command_msg;

double range;
double theta;
double goodDistance = 20;

//rate_hz assignment
double rate_hz = 30;

void getRobotPose(const sensor_msgs::Imu& imu_msg) {
	robot_position.angular.x = imu_msg.orientation.x;	//roll
	robot_position.angular.y = imu_msg.orientation.y;	//pitch
	robot_position.angular.z = imu_msg.orientation.z; 	//yaw
}

void getRange(const sensor_msgs::Range& ultrasound_msg) {
	range = ultrasound_msg.range;
}

void getCommand(const geometry_msgs::Twist& turtle_msg) {

	if (turtle_msg.linear.x == 2 || turtle_msg.linear.x == -2)
	{
		command_msg.angular.z = 0;
		command_msg.linear.x = turtle_msg.linear.x;

	}
	if (command_msg.angular.z == 2 || command_msg.angular.z == -2)
	{
		command_msg.linear.x = command_msg.linear.x;
	}
	command_msg.angular.z = turtle_msg.angular.z;	//Inverted God knows why...
	command_msg.linear.y = turtle_msg.linear.y;
}



geometry_msgs::Twist followTarget(){
	geometry_msgs::Twist velocity;
	if (abs(range - goodDistance) <= 5)
	{
		velocity.linear.x = 90;
		velocity.linear.y = 0;
		velocity.angular.z = 90;

	}else{
		if (range >= goodDistance)
		{
			velocity.linear.x = 180;
			velocity.linear.y = 0;
			velocity.angular.z = 90;

		}else{
			velocity.linear.x = 0;
			velocity.linear.y = 0;
			velocity.angular.z = 90;
		}
	}

	return velocity;
}

geometry_msgs::Twist manualControl(){
	geometry_msgs::Twist velocity_new;

	velocity_new.linear.x = 90*command_msg.linear.x;
	velocity_new.linear.y = command_msg.linear.y;
	if (command_msg.angular.z == -2)
	{
		velocity_new.angular.z = 0;
	}else{
		if (command_msg.angular.z == 0)
		{
			velocity_new.angular.z = 90;
		}else{
			velocity_new.angular.z = 180;
		}
	}

	return velocity_new;
}


int main(int argc, char **argv){
	ros::init(argc,argv,"kontrollieren");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("kontrollieren engaged");
	ROS_INFO_STREAM(ros::this_node::getName());

	ros::Publisher pubKontrolliert = nh.advertise<geometry_msgs::Twist>("/kontrolliert", rate_hz);

	ros::Subscriber cmmds = nh.subscribe("/turtle1/cmd_vel", 1, &getCommand);
	ros::Subscriber ranger = nh.subscribe("/ultrasound", 1, &getRange);
	ros::Subscriber imus = nh.subscribe("/imu", 1, &getRobotPose);

	//Twist variable to publish velocity
	geometry_msgs::Twist desired_velocity;

	double time = 0;
	//define the rate
	ros::Rate rate(rate_hz);

	while (ros::ok())
	{
		//ROS_INFO_STREAM use for debugging
		ROS_INFO_STREAM("Robot Pose"
				<<",X,"<<robot_position.angular.x
				<<",Y,"<<robot_position.angular.y
				<<",W,"<<robot_position.angular.z);
		ROS_INFO_STREAM("Range:"
				<<",ultrasonic range,"<<range);

		// Manual?
		if (command_msg.linear.y != 0.0) {
			desired_velocity = followTarget();
		} else {
			desired_velocity = manualControl();
		}
		//ROS_INFO_STREAM use for debugging
		ROS_INFO_STREAM("Execute command: "
				<<"X:"<<desired_velocity.linear.x
				<<",Y:"<<desired_velocity.linear.y
				<<",W:"<<desired_velocity.angular.z);

		pubKontrolliert.publish(desired_velocity);

		ros::spinOnce();
		rate.sleep();
		time+=(1/rate_hz);
	}
	return 0;
}
