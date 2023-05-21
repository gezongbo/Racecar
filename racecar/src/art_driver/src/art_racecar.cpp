/*********************************
 * 串口节点，订阅cmd_vel话题并发布odometry话题
 * 从cmd_vel话题中分解出速度值通过串口送到移动底盘
 * 从底盘串口接收里程消息整合到odometry话题用于发布
 * 
 * @StevenShi
 * *******************************/
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
// #include <stdio.h>

#define	sBUFFERSIZE	6//send buffer size 串口发送缓存长度
#define	rBUFFERSIZE	27//receive buffer size 串口接收缓存长度
unsigned char s_buffer[sBUFFERSIZE];//发送缓存
unsigned char r_buffer[rBUFFERSIZE];//接收缓存

/************************************
 * 串口数据发送格式共15字节
 * head head linear_v_x  linear_v_y angular_v  CRC
 * 0xff 0xff float       float      float      u8
 * **********************************/
/**********************************************************
 * 串口接收数据格式共27字节
 * head head x-position y-position x-speed y-speed angular-speed pose-angular CRC
 * 0xaa 0xaa float      float      float   float   float         float(yaw)   u8
 * ********************************************************/

//联合体，用于浮点数与16进制的快速转换
typedef union{
	unsigned char cvalue[4];
	float fvalue;
}float_union;

serial::Serial ser;

void swapx(int a)
{
	int b1=(a/16)/16;
	if(b1<10)
		s_buffer[1] = b1;
	else if(b1==10)
		s_buffer[1] = 0xa;
	else if(b1==11)
		s_buffer[1] = 0xb;
	else if(b1==12)
		s_buffer[1] = 0xc;
	else if(b1==13)
		s_buffer[1] = 0xd;
	else if(b1==14)
		s_buffer[1] = 0xe;
	else if(b1==15)
		s_buffer[1] = 0xf;
	int b2=(a-b1*16*16)/16;
	if(b2<10)
		s_buffer[2] = b2;
	else if(b2==10)
		s_buffer[2] = 0xa;
	else if(b2==11)
		s_buffer[2] = 0xb;
	else if(b2==12)
		s_buffer[2] = 0xc;
	else if(b2==13)
		s_buffer[2] = 0xd;
	else if(b2==14)
		s_buffer[2] = 0xe;
	else if(b2==15)
		s_buffer[2] = 0xf;
	int b3=a % 16;
	if(b3<10)
		s_buffer[3] = b3;
	else if(b3==10)
		s_buffer[3] = 0xa;
	else if(b3==11)
		s_buffer[3] = 0xb;
	else if(b3==12)
		s_buffer[3] = 0xc;
	else if(b3==13)
		s_buffer[3] = 0xd;
	else if(b3==14)
		s_buffer[3] = 0xe;
	else if(b3==15)
		s_buffer[3] = 0xf;
}
void swapz(int a)
{
	int b4=a/16;
	while(b4 > 15)
		b4 = b4/16;
	if(b4<10)
		s_buffer[4] = b4;
	else if(b4==10)
		s_buffer[4] = 0xa;
	else if(b4==11)
		s_buffer[4] = 0xb;
	else if(b4==12)
		s_buffer[4] = 0xc;
	else if(b4==13)
		s_buffer[4] = 0xd;
	else if(b4==14)
		s_buffer[4] = 0xe;
	else if(b4==15)
		s_buffer[4] = 0xf;
	int b5 = a % 16;
	if(b5<10)
		s_buffer[5] = b5;
	else if(b5==10)
		s_buffer[5] = 0xa;
	else if(b5==11)
		s_buffer[5] = 0xb;
	else if(b5==12)
		s_buffer[5] = 0xc;
	else if(b5==13)
		s_buffer[5] = 0xd;
	else if(b5==14)
		s_buffer[5] = 0xe;
	else if(b5==15)
		s_buffer[5] = 0xf;
}
/**********************************************************
 * 数据打包，将获取的cmd_vel信息打包并通过串口发送
 * ********************************************************/
void data_pack(const geometry_msgs::Twist& cmd_vel){
	// //unsigned char i;
	// float_union Vx,Vy,Ang_v;
	// Vx.fvalue = cmd_vel.linear.x;
	// Vy.fvalue = cmd_vel.linear.y;
	// Ang_v.fvalue = cmd_vel.angular.z;
	
	memset(s_buffer,0,sizeof(s_buffer));
	// 数据打包
	s_buffer[0] = 0xff;
	swapx(cmd_vel.linear.x);
	swapz(cmd_vel.angular.z);
	 for(int i=0;i<6;i++){
	 ROS_INFO("0x%02x",s_buffer[i]);
	// // 	ROS_INFO("0x%02x",s_buffer[i]);
	 }
	
	ser.write(s_buffer,sBUFFERSIZE);
	
}

//订阅turtle1/cmd_vel话题的回调函数，用于显示速度以及角速度
void cmd_vel_callback(const geometry_msgs::Twist& cmd_vel){
	//ROS_INFO("I heard linear velocity: x-[%f],y-[%f],",cmd_vel.linear.x,cmd_vel.linear.y);
	//ROS_INFO("I heard angular velocity: [%f]",cmd_vel.angular.z);
	//std::cout << "Twist Received" << std::endl;	
	data_pack(cmd_vel);
}
int main (int argc, char** argv){
    ros::init(argc, argv, "art_driver_node");
    ros::NodeHandle nh;

	//订阅/turtle1/cmd_vel话题用于测试 $ rosrun turtlesim turtle_teleop_key
	ros::Subscriber write_sub = nh.subscribe("/car/cmd_vel",1,cmd_vel_callback);
	//发布里程计话题 odom
	// ros::Publisher read_pub = nh.advertise<nav_msgs::Odometry>("odom",1000);

    try
    {
        ser.setPort("/dev/ttyUSB1");
        ser.setBaudrate(230400);  //设置波特率
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }
	ros::spin();

    return 0;
}
