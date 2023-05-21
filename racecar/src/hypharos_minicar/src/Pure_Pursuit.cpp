#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include "nav_msgs/Path.h"
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/Marker.h>
using namespace std;
class SubscribeAndPublish
{
public:
  geometry_msgs::Twist cmd_vel;
  SubscribeAndPublish()
  {
    //Topic you want to publish
    pub_ = n_.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    //Topic you want to subscribe
    sub_ = n_.subscribe("/car/cmd_vel", 10, &SubscribeAndPublish::callback, this);
  }

  void callback(const geometry_msgs::Twist &cmd_vel_buffer)
  {

     //ROS_INFO("1111111car_cmd_vel:%.2f",cmd_vel_buffer.linear.x);
    // float X = cmd_vel_buffer.linear.x;
    // float Z = cmd_vel_buffer.angular.z;
    cmd_vel.linear.x = (cmd_vel_buffer.linear.x - 1500.0) / 100.0;
    if(abs(cmd_vel_buffer.angular.z - 90 ) > 30)
        cmd_vel.angular.z = (cmd_vel_buffer.angular.z - 90.0) / 120.0;
    else
        cmd_vel.angular.z = (cmd_vel_buffer.angular.z - 90.0) / 80.0;
    // printf("%f",cmd_vel.linear.x);
    // printf("%f",cmd_vel.angular.z);
    pub_.publish(cmd_vel);
    // cmd_vel_buffer.linear.x = X;
    // cmd_vel_buffer.angular.z = Z;
    // cmd_vel.linear.x = (cmd_vel_buffer.linear.x - 1500.0) / 100.0;
    // if(abs(cmd_vel_buffer.angular.z - 90 ) > 30)
    //     cmd_vel.angular.z = (cmd_vel_buffer.angular.z - 90.0) / 120.0;
    // else
    //     cmd_vel.angular.z = (cmd_vel_buffer.angular.z - 90.0) / 80.0;
    // cmd_vel.angular.z = cmd_vel.angular.z;
    // ROS_INFO("Gas = %.2f\tSteering angle = %.2f",cmd_vel.linear.x,cmd_vel.angular.z);
    // pub_.publish(cmd_vel);
    // cmd_vel.linear.x = X;
    // cmd_vel.angular.z = Z;
    // pub_.publish(output);
  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_;

};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "PurePursuit");
  
  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;
  ros::spin();
  return 0;
}