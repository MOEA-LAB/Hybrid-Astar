#include "serial_ros.h"
#include "nlohmann/json.hpp"

#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/Bool.h>
#include "serial_ros/PoseMsgs.h"
#include <vector>

/**
 * 类构造函数，用于初始化ROS中的节点(node)和话题(topic)
 */
SerialRos::SerialRos(ros::NodeHandle &n)
{
    // 获取需要的参数信息
    n.param<std::string>("name", m_serialName, "");
    n.param<int>("baud", m_serialBaud, 0);
    n.getParam("comm_hub/vehicle/name", m_vehicleName);
    std::cout << m_serialName << m_serialBaud << "\n";

    // 打开串口
    m_ser.setPort("/dev/ttyUSB0"); // 更改为你的串口设备
    m_ser.setBaudrate(57600);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    m_ser.setTimeout(to);
    try
    {
        m_ser.open();
    }
    catch (serial::IOException &e)
    {
        ROS_ERROR("Unable to open serial port!");
        return;
    }
    if (m_ser.isOpen())
        ROS_INFO("Serial port opened");
    else
        return;

    // 初始化需要发布的话题(topic)
    m_posePublisher = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/motion_capture_pose", 10);
    m_posePublisher_all = n.advertise<serial_ros::PoseMsgs>("/motion_capture_pose_all", 10);
    ctrl_run_pub = n.advertise<std_msgs::Bool>("/ctrl_run", 10);
    ctrl_speed_pub = n.advertise<std_msgs::UInt32>("/ctrl_speed", 10);
    ctrl_switch_mode_pub = n.advertise<std_msgs::UInt32>("/ctrl_switch_mode", 10);

    // 开启数据接收解析线程
    serialSendThread_ = std::thread(&SerialRos::_serialSendThread, this);
}

SerialRos::~SerialRos()
{
}

/**
 * 独立线程，用于数据接收及协议解析(详细数据协议见wiki中的 第三章-通信说明-无线串口通信)
 */
void SerialRos::_serialSendThread()
{
    int count = 0;
    bool isHeader = false;
    bool isHeaderOrFooter = false;
    uint8_t recvDataAll[500];
    memset(recvDataAll, 0, sizeof(recvDataAll));
    while (ros::ok())
    {
        uint8_t recvData[1];
        m_ser.read(recvData, sizeof(recvData));
        if (recvData[0] == 0x7E && !isHeaderOrFooter)
        {
            isHeaderOrFooter = true;
            continue;
        }

        if (isHeaderOrFooter)
        {
            if (recvData[0] == 0x7E)
            {
                isHeader = true;
            }
            else if (recvData[0] == 0x7D)
            {
                if (count > 8)
                {
                    parseData(recvDataAll, count);
                }
                count = 0;
                memset(recvDataAll, 0, sizeof(recvDataAll));
                isHeader = false;
            }
            else
            {
                recvDataAll[count] = 0x7E;
                count += 1;
                recvDataAll[count] = recvData[0];
                count += 1;
            }
            isHeaderOrFooter = false;
            continue;
        }

        if (isHeader)
        {
            recvDataAll[count] = recvData[0];
            count += 1;
        }
    }
}

/**
 * 数据区数据解析
 *
 * @param data 数据区内容
 * @param size 数据区长度
 * @return 无
 */
void SerialRos::parseData(const uint8_t *data, size_t size)
{

    // 打印十六进制接收数据(用于测试)
    /*     for (size_t i = 0; i < size; i++)
        {
            printf("%02x ", data[i]);
            if(i%10 == 1){
                printf("\n");
            }
        }
        printf("\n"); */

    // crc校验
    uint16_t crcResult = calculateCRC16X25(data + 2, size - 4);
    if ((crcResult & 0xFF) == data[size - 2] && ((crcResult >> 8) & 0xFF) == data[size - 1])
    {
        // printf("CRC16 校验通过\n");
    }
    else
    {
        printf("CRC16 校验失败\n");
        return;
    }

    // 数据解析(详细数据协议见wiki中的 第三章-通信说明-无线串口通信)
    int len = (static_cast<int>(data[1]) << 8) | static_cast<int>(data[0]);
    int ptr = 2;
    // 21辆小车信息，有需要可以增加
    std::vector<geometry_msgs::PoseWithCovarianceStamped> PoseMsgs_v(21);
    while (ptr < len - 2)
    {
        // 序号
        auto id = static_cast<int>(data[ptr]);
        ptr += 1;

        // x坐标(为方便传输已放大100倍)
        int xTmp = (data[ptr + 1] << 8) | data[ptr];
        ptr += 2;

        // y坐标(为方便传输已放大100倍)
        int yTmp = (data[ptr + 1] << 8) | data[ptr];
        ptr += 2;

        // 朝向角(为方便传输已放大100倍)
        int yawTmp = (data[ptr + 1] << 8) | data[ptr];
        ptr += 2;

        float x = xTmp / 100.0;
        float y = yTmp / 100.0;
        float yaw = yawTmp / 100.0;

        // 启动/停止
        bool isRun = (data[ptr] & (1 << 7)) != 0;
        ptr += 1;

        // 数据打包成话题(topic)并发布
        tf2::Quaternion quat;
        quat.setRPY(0, 0, yaw);
        geometry_msgs::PoseWithCovarianceStamped poseMsg;
        poseMsg.header.stamp = ros::Time::now();
        poseMsg.header.frame_id = "odom";
        poseMsg.header.seq = m_seq++;
        poseMsg.pose.pose.position.x = x;
        poseMsg.pose.pose.position.y = y;
        poseMsg.pose.pose.position.z = 0.0;
        poseMsg.pose.pose.orientation.x = quat.x();
        poseMsg.pose.pose.orientation.y = quat.y();
        poseMsg.pose.pose.orientation.z = quat.z();
        poseMsg.pose.pose.orientation.w = quat.w();
        poseMsg.pose.covariance = {1e-4, 0, 0, 0, 0, 0,
                                   0, 1e-4, 0, 0, 0, 0,
                                   0, 0, 1e-4, 0, 0, 0,
                                   0, 0, 0, 1e-4, 0, 0,
                                   0, 0, 0, 0, 1e-4, 0,
                                   0, 0, 0, 0, 0, 1e-4};
        PoseMsgs_v[id - 70] = poseMsg;
        if (m_vehicleName == "Car" + std::to_string(id))
        {
            m_posePublisher.publish(poseMsg);

            std_msgs::Bool msg;
            msg.data = isRun;
            ctrl_run_pub.publish(msg);
        }
    }
    serial_ros::PoseMsgs PoseMsgs;
    PoseMsgs.poseMsgs = PoseMsgs_v;
    PoseMsgs.carId = m_vehicleName;
    m_posePublisher_all.publish(PoseMsgs);
}

static const unsigned short crc16tab[] = {
    0x0000,
    0x1021,
    0x2042,
    0x3063,
    0x4084,
    0x50a5,
    0x60c6,
    0x70e7,
    0x8108,
    0x9129,
    0xa14a,
    0xb16b,
    0xc18c,
    0xd1ad,
    0xe1ce,
    0xf1ef,
};

/**
 * 计算crc校验值
 *
 * @param data 校验内容
 * @param length 校验长度
 * @return crc校验值
 */
uint16_t SerialRos::calculateCRC16X25(const uint8_t *data, size_t length)
{
    unsigned short crc = 0;
    unsigned char ch = 0;

    while (length-- != 0)
    {
        ch = crc >> 12;
        crc <<= 4;
        crc ^= crc16tab[ch ^ (*data / 16)];

        ch = crc >> 12;
        crc <<= 4;
        crc ^= crc16tab[ch ^ (*data & 0x0f)];
        data++;
    }
    return crc;
}

/**
 * bytes转float
 *
 * @param data 转换内容
 * @param length 转换长度
 * @return 转换结果
 */
float SerialRos::bytesToFloat(const uint8_t *data, size_t length)
{
    float result;
    std::memcpy(&result, data, sizeof(float));
    return result;
}