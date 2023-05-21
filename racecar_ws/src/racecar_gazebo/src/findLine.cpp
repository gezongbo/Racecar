#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include "std_msgs/Float64.h"
#include "sensor_msgs/Image.h"
#include "geometry_msgs/Twist.h"
#include "ackermann_msgs/AckermannDrive.h"
#include "ackermann_msgs/AckermannDriveStamped.h"
#include <opencv-3.3.1-dev/opencv2/core.hpp>
#include <opencv-3.3.1-dev/opencv/highgui.h>
#include <opencv-3.3.1-dev/opencv2/opencv.hpp>
#include "cv_bridge/cv_bridge.h"
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Imu.h>
#include <image_transport/image_transport.h>
#include <trajectory_msgs/JointTrajectory.h>

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

using namespace cv;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
class Follower{

    public:
	ros::NodeHandle node;
	static ros::Publisher cmdpub;
	static ros::Subscriber imageSub;

	static void image_callback(const sensor_msgs::ImageConstPtr& msg);
	static void speed_contrl(float speed_car,float angluar_car);
	Follower();
};


ros::Publisher Follower::cmdpub;
ros::Subscriber Follower::imageSub;

int main(int argc, char* argv[])
{
	ros::init(argc,argv,"follower_cpp");
	Follower f;//调用构造函数
        MoveBaseClient ac("move_base", true);
        
  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
 
  move_base_msgs::MoveBaseGoal goal_1;
  
  goal_1.target_pose.header.frame_id = "map";
  
  goal_1.target_pose.header.stamp = ros::Time::now();
        //Goal one
        goal_1.target_pose.pose.position.x = 9.92; 
        goal_1.target_pose.pose.position.y = -0.063;  
        goal_1.target_pose.pose.position.z = 0;   
        goal_1.target_pose.pose.orientation.w = 1; 
  ROS_INFO("Sending Goal one!");
  ac.sendGoal(goal_1);
   ac.waitForResult();
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("You have reached the Goal one!");
  else
    ROS_INFO("You failed reached the goal!");

	while(ros::ok())
	{
		ros::Rate loop_rate(0.2);
		//这里是程序的循环处，可以写你的代码
                f.speed_contrl;
		ros::spinOnce();
	}

}

void Follower::image_callback(const sensor_msgs::ImageConstPtr& msg)
{
	cv_bridge::CvImagePtr cv_ptr;
	
        try
        {
            cv_ptr =  cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); 
        }
        catch(cv_bridge::Exception& e)  
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
	}
	Mat hsv = cv_ptr->image.clone();
	Mat mask = cv_ptr->image.clone();
	cvtColor(cv_ptr->image, hsv, COLOR_BGR2HSV);
        double low_H = 0;
        double low_S = 0;
        double low_V = 100;
        double high_H = 180;
        double high_S = 30;
        double high_V = 255;
	inRange(hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), mask);

	speed_contrl(2.5,1.5);

	imshow("mask",mask);
	waitKey(3);
}



void Follower::speed_contrl(float speed_car,float angluar_car)
{
    ackermann_msgs::AckermannDriveStamped ack;
    ack.drive.speed = speed_car;
    ack.drive.steering_angle = angluar_car;
    cmdpub.publish(ack);
}


Follower::Follower()
{
	cmdpub = node.advertise<ackermann_msgs::AckermannDriveStamped>("/vesc/low_level/ackermann_cmd_mux/input/teleop", 10, true);
	imageSub = node.subscribe("/camera/zed/rgb/image_rect_color",10,image_callback);
}
