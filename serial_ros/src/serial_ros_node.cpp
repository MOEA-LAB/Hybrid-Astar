#include "ros/ros.h"
#include "serial_ros.h"

int main(int argc, char **argv)
{
    // 防止乱码
    setlocale(LC_ALL, "");

    // 初始化“serial_ros_node”节点
    ros::init(argc, argv, "serial_ros_node");
    ros::NodeHandle n;

    // 启动“serial_ros_node”节点
    ROS_INFO("serial_ros_node start");
    SerialRos node(n);

    ros::spin();

    return 0;
}
