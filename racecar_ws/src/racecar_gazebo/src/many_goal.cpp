#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "ackermann_msgs/AckermannDrive.h"
#include "ackermann_msgs/AckermannDriveStamped.h"
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
/*ackermann_msgs::AckermannDriveStamped ack;
ros::Publisher cmd_vel_pub;
void Callback(const geometry_msgs::Twist& msg)
{
    //ROS_INFO("Get goal CMD:%d",msg->data); 
    ack.drive.speed=msg.linear.x;
    ack.drive.steering_angle=msg.angular.z;
    cmd_vel_pub.publish(ack);
}*/
int main(int argc, char** argv){
  ros::init(argc, argv, "many_goal");
   /*ros::NodeHandle ndHandle;
   ros::Subscriber cmd_vel_sub = ndHandle.subscribe("/cmd_vel", 10, Callback);
   cmd_vel_pub  = ndHandle.advertise<ackermann_msgs::AckermannDriveStamped>("/vesc/low_level/ackermann_cmd_mux/input/teleop", 10);
   ack.drive.speed=50.0;
   ack.drive.steering_angle=1.34;*/
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);
 
  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
 
  move_base_msgs::MoveBaseGoal goal_1;
  
  goal_1.target_pose.header.frame_id = "map";
  
  goal_1.target_pose.header.stamp = ros::Time::now();
        //Goal one
        goal_1.target_pose.pose.position.x = -1.42; 
        goal_1.target_pose.pose.position.y = 1.92;  
        goal_1.target_pose.pose.position.z = 0;   
        goal_1.target_pose.pose.orientation.w = 1; 
  ROS_INFO("Sending Goal one!");
  ac.sendGoal(goal_1);
   ac.waitForResult();
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("You have reached the Goal one!");
  else
    ROS_INFO("You failed reached the goal!");
  
  ros::spin();
  return 0;
}

        
