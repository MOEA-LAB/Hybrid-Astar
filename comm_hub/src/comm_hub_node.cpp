// udp_client_node.cpp
#include "ros/ros.h"
#include "comm_hub.h"

int main(int argc, char **argv)
{
    // 防止乱码
    setlocale(LC_ALL, "");

    // 初始化“comm_hub_node”节点
    ros::init(argc, argv, "comm_hub_node");
    ros::NodeHandle n;

    // 启动“comm_hub_node”节点
    ROS_INFO("comm_hub_node start");
    CommHub node(n);

    ros::spin();

    return 0;
}
