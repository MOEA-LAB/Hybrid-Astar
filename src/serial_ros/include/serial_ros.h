#ifndef SERIAL_ROS_H
#define SERIAL_ROS_H

// 标准库
#include <string>
#include <iostream>
#include <thread>

// ros
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>

class SerialRos
{
public:
    SerialRos(ros::NodeHandle &n);
    ~SerialRos();

private:
    std::string m_vehicleName;

    serial::Serial m_ser;
    std::string m_serialName;
    int m_serialBaud;

    std::thread serialSendThread_;
    void _serialSendThread();
    void parseData(const uint8_t *data, size_t size);

    ros::Publisher m_posePublisher;
    ros::Publisher m_posePublisher_all;
    ros::Publisher ctrl_run_pub;
    ros::Publisher ctrl_speed_pub;
    ros::Publisher ctrl_switch_mode_pub;
    int m_seq;

    uint16_t calculateCRC16X25(const uint8_t *data, size_t length);
    float bytesToFloat(const uint8_t *data, size_t length);
};

#endif