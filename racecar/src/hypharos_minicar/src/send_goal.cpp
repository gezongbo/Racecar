#include<iostream>
#include <ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<geometry_msgs/Point.h>
#include"std_msgs/Int32.h"

using namespace std;

class Goal{
public:
    geometry_msgs::PoseStamped goal_1;
    geometry_msgs::PoseStamped goal_2;
    geometry_msgs::PoseStamped goal_3;
    geometry_msgs::PoseStamped goal_4;
    int m=1;

    Goal(){
        pub=n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",10);
        sub=n.subscribe("int",1,&Goal::callback,this);

        goal_1.header.frame_id = "map";
        goal_2.header.frame_id = "map";
        goal_3.header.frame_id = "map";
        goal_4.header.frame_id = "map";
        //Goal one
        goal_1.pose.position.x = 11.4;
        goal_1.pose.position.y = -1.9;
        goal_1.pose.orientation.w = 1.0;

        //Goal two
        goal_2.pose.position.x = 21.0;
        goal_2.pose.position.y = -3.1;
        goal_2.pose.orientation.w = 1.0;

        //Goal three
        goal_3.pose.position.x = 28.8;
        goal_3.pose.position.y = -2.4;
        goal_3.pose.orientation.w = 1.0;


        //Goal four
        goal_4.pose.position.x = 26.0;
        goal_4.pose.position.y = 7.5;
        goal_4.pose.orientation.w = 1.0;
    }

private:
    ros::NodeHandle n; 
    ros::Publisher pub;
    ros::Subscriber sub;
    void callback(const std_msgs::Int32 i);
    
};


void Goal::callback(const std_msgs::Int32 i)
{ 
     //ROS_INFO("i=:%d",i.data);
    m=i.data;
    //ROS_INFO("m=:%d",m);
    if(m==1){
        ROS_INFO("Send goal one");
        pub.publish(goal_1);
    }
    if(m==2){
        ROS_INFO("Send goal two");
        pub.publish(goal_2);
    }
    if(m==3){
        ROS_INFO("Send goal three");
        pub.publish(goal_3);
    }
    if(m==4){
        ROS_INFO("Send goal four");
        pub.publish(goal_4);
    }
}

int main(int argc,char **argv)
{
    ros::init(argc,argv,"many_goal");

    Goal g;

    ros::spin();
    return 0;
}
