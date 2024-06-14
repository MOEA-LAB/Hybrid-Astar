#ifndef COMM_HUB_H
#define COMM_HUB_H

#include "myUdp.h"

// 标准库
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

// ros
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/TwistStamped.h>

struct Vehicle
{
    double x;                           // 动捕x
    double y;                           // 动捕y
    double yaw;                         // 动捕朝向
    double powerVoltage;                // 电池电压
    sensor_msgs::Imu imu;               // imu数据
    nav_msgs::Odometry odom;            // odom（里程计）数据
    geometry_msgs::TwistStamped cmdVel; // 下发至底盘驱动的数据
};

class CommHub
{
public:
    // 构造函数: port--udp监听端口
    CommHub(ros::NodeHandle &n, unsigned short port = 30000);
    ~CommHub();

private:
    // 配置文件参数
    std::string m_nodeName;
    std::string m_serverIp;
    int m_serverPort;
    std::string m_vehicleName;

    // 车辆信息
    Vehicle m_vehicle;

    // ros接收消息回调
    ros::Subscriber power_sub;
    ros::Subscriber pose_sub;
    ros::Subscriber imu_sub;
    ros::Subscriber odom_sub;
    ros::Subscriber cmd_vel_sub;
    void _PowerVoltageCallback(const std_msgs::Float32ConstPtr &msg);
    void _PoesCallback(const geometry_msgs::PoseWithCovarianceStamped &msg);
    void _ImuCallback(const sensor_msgs::Imu &msg);
    void _OdomCallback(const nav_msgs::Odometry &msg);
    void _CmdVelCallback(const geometry_msgs::TwistStamped &msg);

    // ros::Publisher ctrl_run_pub;
    // ros::Publisher ctrl_speed_pub;
    // ros::Publisher ctrl_switch_mode_pub;

    // 收发线程
    MyUdp m_udp;
    std::thread sendThread_;
    std::thread readThread_;
    void _sendThread();
    void _readThread();
    void parseData(const std::string &data);
};

#endif