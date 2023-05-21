#include<iostream>
#include <ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
using namespace std;

int flag=1;
int g1=0,g2=0,g3=0,g4=0;

class Goal{
public:
    geometry_msgs::Twist goal_cmd_vel;

    geometry_msgs::PoseStamped goal_1;
    geometry_msgs::PoseStamped goal_2;
    geometry_msgs::PoseStamped goal_3;
    geometry_msgs::PoseStamped goal_4;

    Goal(){
        pub=n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",10);
        sub=n.subscribe("/cmd_vel",10,&Goal::callback,this);

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

    void callback(const geometry_msgs::Twist &v){


        //发送第一个目标点，如果发送成功，v将大于0
        if(flag==1&&v.linear.x==0){
            ROS_INFO("Sending goal one");
            pub.publish(goal_1);
            g1=1;
        }
        
        if(v.linear.x>0&&flag==1)
            flag=2;

        if(flag==2&&v.linear.x==0&&g1){
             ROS_INFO("Sending goal two");
             pub.publish(goal_2);
             g2=1;
        }

        if(v.linear.x>0&&flag==2&&g2)
            flag=3;

        if(flag==3&&v.linear.x==0&&g2){
             ROS_INFO("Sending goal three");
             pub.publish(goal_3);
             g3=1;
        }

        if(v.linear.x>0&&flag==3&&g3)
            flag=4;

        if(flag==4&&v.linear.x==0&&g3){
             ROS_INFO("Sending goal four");
             pub.publish(goal_4);
        }
    }

private:
    ros::NodeHandle n; 
    ros::Publisher pub;
    ros::Subscriber sub;
};
int main(int argc,char **argv)
{
    ros::init(argc,argv,"many_goal");

    Goal g;

    ros::spin();
    return 0;
}