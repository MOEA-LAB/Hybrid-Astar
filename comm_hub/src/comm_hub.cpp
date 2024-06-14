#include "comm_hub.h"
#include "myMsg.h"
#include "nlohmann/json.hpp"

// ros
#include <tf/transform_datatypes.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/Bool.h>

CommHub::CommHub(ros::NodeHandle &n, unsigned short port) : m_udp(port)
{
    // udp初始化
    m_udp.CreateSocket();
    if (!m_udp.Bind())
        ROS_INFO("bind error!\n");

    // 配置文件参数提取
    m_nodeName = ros::this_node::getName();
    n.getParam(m_nodeName + "/server/ip", m_serverIp);
    n.getParam(m_nodeName + "/server/port", m_serverPort);
    n.getParam(m_nodeName + "/vehicle/name", m_vehicleName);
    ROS_INFO("m_serverIp: %s", m_serverIp.c_str());
    ROS_INFO("m_serverPort: %d", m_serverPort);

    // ros接收消息回调初始化
    power_sub = n.subscribe("/PowerVoltage", 10, &CommHub::_PowerVoltageCallback, this);
    pose_sub = n.subscribe("/vrpn_client_node/" + m_vehicleName + "/pose", 10, &CommHub::_PoesCallback, this);
    imu_sub = n.subscribe("/imu", 10, &CommHub::_ImuCallback, this);
    odom_sub = n.subscribe("/odom", 10, &CommHub::_OdomCallback, this);
    cmd_vel_sub = n.subscribe("/cmd_vel", 10, &CommHub::_CmdVelCallback, this);

    // 自定义消息格式较为麻烦，直接分开传输
    // ctrl_run_pub = n.advertise<std_msgs::Bool>("/ctrl_run", 10);
    // ctrl_speed_pub = n.advertise<std_msgs::UInt32>("/ctrl_speed", 10);
    // ctrl_switch_mode_pub = n.advertise<std_msgs::UInt32>("/ctrl_switch_mode", 10);

    // udp收发线程初始化
    sendThread_ = std::thread(&CommHub::_sendThread, this);
    readThread_ = std::thread(&CommHub::_readThread, this);
}

CommHub::~CommHub()
{
}

// 电池电量
void CommHub::_PowerVoltageCallback(const std_msgs::Float32ConstPtr &msg)
{
    m_vehicle.powerVoltage = msg->data;
    // ROS_INFO("power voltage: %lf", m_vehicle.powerVoltage);
}

void CommHub::_PoesCallback(const geometry_msgs::PoseWithCovarianceStamped &msg)
{
    // 四元素转欧拉角
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg.pose.pose.orientation, quat);
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);

    m_vehicle.x = msg.pose.pose.position.x;
    m_vehicle.y = msg.pose.pose.position.y;
    m_vehicle.yaw = yaw;

    // ROS_INFO("x: %lf, x: %lf, yaw: %lf", x, x, yaw);
}

void CommHub::_ImuCallback(const sensor_msgs::Imu &msg)
{
    m_vehicle.imu = msg;
}

void CommHub::_OdomCallback(const nav_msgs::Odometry &msg)
{
    m_vehicle.odom = msg;
}

void CommHub::_CmdVelCallback(const geometry_msgs::TwistStamped &msg)
{
    m_vehicle.cmdVel = msg;
}

void CommHub::_sendThread()
{
    while (1)
    {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();

        nlohmann::json jImu;
        jImu["orientation_x"] = m_vehicle.imu.orientation.x;
        jImu["orientation_y"] = m_vehicle.imu.orientation.y;
        jImu["orientation_z"] = m_vehicle.imu.orientation.z;
        jImu["orientation_w"] = m_vehicle.imu.orientation.w;

        nlohmann::json jOdom;
        jOdom["position_x"] = m_vehicle.odom.pose.pose.position.x;
        jOdom["linear_x"] = m_vehicle.odom.twist.twist.linear.x;

        nlohmann::json jCmdVel;
        jCmdVel["v"] = m_vehicle.cmdVel.twist.linear.x;
        jCmdVel["angular"] = m_vehicle.cmdVel.twist.angular.z;

        nlohmann::json jData;
        jData["name"] = m_vehicleName;
        jData["x"] = m_vehicle.x;
        jData["y"] = m_vehicle.y;
        jData["yaw"] = m_vehicle.yaw;
        jData["power_voltage"] = m_vehicle.powerVoltage;
        jData["imu"] = jImu;
        jData["odom"] = jOdom;
        jData["cmd_vel"] = jCmdVel;

        nlohmann::json j;
        j["from"] = m_vehicleName;
        j["to"] = "collaborative_server";
        j["tick"] = std::to_string(timestamp);
        j["action"] = "vehicle_info";
        j["data"] = jData;

        auto jStr = j.dump();
        auto sendStr = send_msg_package(jStr);
        m_udp.Send(sendStr.data(), sendStr.size(), m_serverIp.data(), m_serverPort);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void CommHub::_readThread()
{
    // buf初始化
    char recvData[2048];
    char recvDataAll[2048 * 10];
    memset(recvData, 0, sizeof(recvData));
    memset(recvDataAll, 0, sizeof(recvDataAll));
    int allLen = 0;

    while (ros::ok())
    {
        // 接收buf清空
        memset(recvData, 0, sizeof(recvData));
        // 接收数据
        auto len = m_udp.Recv(recvData, sizeof(recvData));
        // 将buf内数据放至在一个更大的buf中（为了处理粘包和断包的现象）
        memcpy(recvDataAll + allLen, recvData, len);
        allLen += len;

        // 数据解析
        std::vector<std::string> recvJsons;
        int parseLen = recv_msg_parse_1(recvData, len, recvJsons);
        if (parseLen == 0)
        {
            continue;
        }

        // json解析
        for (auto recvJson : recvJsons)
        {
            parseData(recvJson);
        }

        // 将已解析数据从buf中删除
        memcpy(recvDataAll, recvDataAll + parseLen, allLen - parseLen);
        memset(recvDataAll + allLen - parseLen, 0, parseLen);
        allLen = allLen - parseLen;
    }
}

void CommHub::parseData(const std::string &data)
{
    nlohmann::json j;
    try
    {
        j = nlohmann::json::parse(data);
    }
    catch (nlohmann::json::parse_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        auto action = j["action"].get<std::string>();
        // ROS_INFO("action: %s", action);
        // if (action == "car_switch_mode")
        // {
        //     auto mode = j["data"]["mode"].get<int>();
        //     std_msgs::UInt32 msg;
        //     msg.data = static_cast<uint32_t>(mode);
        //     ctrl_switch_mode_pub.publish(msg);
        // }
        // else if (action == "car_start")
        // {
        //     std_msgs::Bool msg;
        //     msg.data = true;
        //     ctrl_run_pub.publish(msg);
        // }
        // else if (action == "car_stop")
        // {
        //     std_msgs::Bool msg;
        //     msg.data = false;
        //     ctrl_run_pub.publish(msg);
        // }
        // else if (action == "car_speed")
        // {
        //     auto speed = j["data"]["speed"].get<int>();
        //     std_msgs::UInt32 msg;
        //     msg.data = static_cast<uint32_t>(speed);
        //     ctrl_speed_pub.publish(msg);
        // }
    }
    catch (nlohmann::json::out_of_range &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}
