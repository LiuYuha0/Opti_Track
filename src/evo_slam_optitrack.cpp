#include<iostream>
#include<fstream>
#include<ros/ros.h>
#include "OptiTrackClient.hpp"
#include<nav_msgs/Odometry.h>

using namespace std;

//定义时间戳
double trac_stamp;

// 
void odometry_save_callback(const nav_msgs::Odometry::ConstPtr& odom_msg)
{
	nav_msgs::Odometry odom = *odom_msg;
	//odom_stamp = odom.header.stamp.toSec();

	ofstream slam_path_file("/home/user/Documents/ORBSLAM3.txt", ios::app);
	slam_path_file.setf(ios::fixed, ios::floatfield);
	slam_path_file.precision(0);
	slam_path_file << trac_stamp * 1e9 << " ";
	slam_path_file.precision(10);
	slam_path_file 	<< odom.pose.pose.position.x << " "
			<< odom.pose.pose.position.y << " "
			<< odom.pose.pose.position.z << " "
			<< odom.pose.pose.orientation.x << " "
			<< odom.pose.pose.orientation.y << " "
			<< odom.pose.pose.orientation.z << " "
			<< odom.pose.pose.orientation.w << endl;
	slam_path_file.close();
}


void optiTrack_save_callback(const opti_track::TrackData::ConstPtr& trac_msg)
{
	opti_track::TrackData trac = *trac_msg;
	trac_stamp = trac.header.stamp.toSec();

	ofstream optitrack_path_file("/home/user/Documents/OptiTrack.txt", ios::app);
	optitrack_path_file.setf(ios::fixed, ios::floatfield);
	optitrack_path_file.precision(0);
	optitrack_path_file << trac_stamp * 1e9 << " ";
	optitrack_path_file.precision(10);
	optitrack_path_file << trac_msg->rigid_bodies[0].pose.position.x << " "
			<< trac_msg->rigid_bodies[0].pose.position.y << " "
			<< trac_msg->rigid_bodies[0].pose.position.z << " "
			<< trac_msg->rigid_bodies[0].pose.orientation.x << " "
			<< trac_msg->rigid_bodies[0].pose.orientation.y << " "
			<< trac_msg->rigid_bodies[0].pose.orientation.z << " "
			<< trac_msg->rigid_bodies[0].pose.orientation.w << endl;
	optitrack_path_file.close();

}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "evo_slam_optitrack");

	ros::NodeHandle nh("~");


    ros::Subscriber sub_odom = nh.subscribe<nav_msgs::Odometry>("/Mono_Inertial/odometry", 100, odometry_save_callback); 
    ros::Subscriber sub_optiTrack = nh.subscribe<opti_track::TrackData>("/opti_track_node/opti_track_data", 100, optiTrack_save_callback);


	ros::spin();

	return 0;
}