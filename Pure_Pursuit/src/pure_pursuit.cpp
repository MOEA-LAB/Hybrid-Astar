#include "pure_pursuit.h"

#include <geometry_msgs/TwistStamped.h>
#include <tf/transform_datatypes.h>
#include <fstream>

#include <nlohmann/json.hpp>

#define PI 3.1415926

pure_pursuit::pure_pursuit(ros::NodeHandle &n) : nh(n)
{
    // m_nodeName = ros::this_node::getName();
    n.getParam("comm_hub/vehicle/name", m_vehicleName);
    n.getParam("comm_hub/vehicle/debug", m_isRun);
    n.getParam("comm_hub/vehicle/speed", currenV);
    n.getParam("comm_hub/vehicle/route", m_routeFileName);
    n.getParam("comm_hub/vehicle/angle_error", m_angleError);
    loadRoute(m_routeFileName);

    lhd = 0.333 * currenV + 0.3;     //Leading Hold Distance
    printf("lhd: %f\n", lhd);
    //变量初始化,避免还未接受到位置信息时调用越界
    std::vector<geometry_msgs::PoseWithCovarianceStamped> poseMsgs(21);
    CarsInfo.poseMsgs=poseMsgs;

    // 订阅节点
    vrpn_sub = n.subscribe("/motion_capture_pose", 10, &pure_pursuit::vrpnCallback, this);
    //订阅全部智能车位置信息
    vrpn_sub_all = n.subscribe("/motion_capture_pose_all", 10, &pure_pursuit::vrpnCallback_all, this);
    // 订阅传感器ekf融合数据
    ctrl_run_sub = n.subscribe("/ctrl_run", 10, &pure_pursuit::ctrl_run_callback, this);
    ctrl_speed_pub = n.subscribe("/ctrl_speed", 10, &pure_pursuit::ctrl_speed_callback, this);
    ctrl_switch_mode_pub = n.subscribe("/ctrl_switch_mode", 10, &pure_pursuit::ctrl_switch_mode_callback, this);

    imu_sub = n.subscribe("/imu", 10, &pure_pursuit::_ImuCallback, this);
    odom_sub = n.subscribe("/odom", 10, &pure_pursuit::_OdomCallback, this);

    // 发布控制节点
    control_pub = n.advertise<geometry_msgs::TwistStamped>("/cmd_vel", 10, true);

    controlPubThread_ = std::thread(&pure_pursuit::_controlPub, this);
}

pure_pursuit::~pure_pursuit()
{
}
//全部智能车位置信息回调函数
void pure_pursuit::vrpnCallback_all(const serial_ros::PoseMsgs &msg)
{
  //接受到信息后赋值给全局变量
  CarsInfo=msg;
}

void pure_pursuit::vrpnCallback(const geometry_msgs::PoseWithCovarianceStamped &msg)
{
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg.pose.pose.orientation, quat);
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
    yaw = yaw + PI / 2 + m_angleError;

    State x0;
    x0.phi = yaw;
    x0.X = msg.pose.pose.position.x;
    x0.Y = msg.pose.pose.position.y;

    if (x0.X < 0 || x0.X > 16 || x0.Y < 0 || x0.Y > 16)
    {
        return;
    }

    if (baseAngle == -10)
    {
        baseAngle = x0.phi;
    }

    baseAngle += x0.phi - imu[0];


    for (size_t i = 0; i < sizeof(imu) / sizeof(imu[0]); i++)
    {
        x0.phi = imu[i];
        x0.X += odom[i] * cos(imu[i]) * 0.02;
        x0.Y += odom[i] * sin(imu[i]) * 0.02;
    }

    m_x = x0.X;
    m_y = x0.Y;
    m_yaw = x0.phi;
}

void pure_pursuit::ekfStateCallback(const nav_msgs::OdometryConstPtr &msg)
{
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg->pose.pose.orientation, quat);
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);

    State x0;
    x0.phi = yaw + PI / 2;
    x0.X = msg->pose.pose.position.x;
    x0.Y = msg->pose.pose.position.y;
    x0.vx = msg->twist.twist.linear.x;
    x0.vy = msg->twist.twist.linear.y;
    x0.r = msg->twist.twist.angular.z;


    Eigen::Vector2d targetPos;
    if (m_mode)
    {
    }
    else
    {
        double dist = track_.porjectOnSpline(x0);
        dist += lhd;
        targetPos = track_.getPostion(dist);
    }

    if (0 > targetPos[0] || targetPos[1] < 0)
    {
        return;
    }

    // 纯跟踪计算
    OutPut u0 = calcPurePursuit(x0, targetPos);

    cmd_vel_linear = u0.V;
    cmd_vel_angular = u0.W;
}

void pure_pursuit::_ImuCallback(const sensor_msgs::Imu &msg)
{
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg.orientation, quat);
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
    yaw += baseAngle;
    if (yaw < -PI)
    {
        yaw += 2 * PI;
    }
    else if (yaw > PI)
    {
        yaw -= 2 * PI;
    }

    m_yaw = yaw;


    for (size_t i = 0; i < sizeof(imu) / sizeof(imu[0]) - 1; i++)
    {
        imu[i] = imu[i + 1];
    }
    imu[sizeof(imu) / sizeof(imu[0]) - 1] = yaw;
}

void pure_pursuit::_OdomCallback(const nav_msgs::Odometry &msg)
{

    m_x += odom[sizeof(odom) / sizeof(odom[0]) - 1] * cos(m_yaw) * 0.02;
    m_y += odom[sizeof(odom) / sizeof(odom[0]) - 1] * sin(m_yaw) * 0.02;

    for (size_t i = 0; i < sizeof(odom) / sizeof(odom[0]) - 1; i++)
    {
        odom[i] = odom[i + 1];
    }
    odom[sizeof(odom) / sizeof(odom[0]) - 1] = msg.twist.twist.linear.x;
}

void pure_pursuit::ctrl_run_callback(const std_msgs::Bool::ConstPtr &msg)
{
    m_isRun = msg->data;
}

void pure_pursuit::ctrl_speed_callback(const std_msgs::UInt32::ConstPtr &msg)
{
    m_speed = msg->data;
}

void pure_pursuit::ctrl_switch_mode_callback(const std_msgs::UInt32::ConstPtr &msg)
{
    m_mode = msg->data;
}

void pure_pursuit::_controlPub()
{
    while (1)
    {
        State x0;
        x0.phi = m_yaw;
        x0.X = m_x;
        x0.Y = m_y;

        Eigen::Vector2d targetPos;
        if (m_mode)
        {
        }
        else
        {
            double dist = track_.porjectOnSpline(x0);
            dist += lhd;
            targetPos = track_.getPostion(dist);
        }

        if (0 > targetPos[0] || targetPos[1] < 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return;
        }
        //获取某辆车位置信息demo(Car88为例,Id为18)
        double Car_x=CarsInfo.poseMsgs[18].pose.pose.position.x;
        double Car_y=CarsInfo.poseMsgs[18].pose.pose.position.y;
        double Car_yaw=quaternionToYaw(CarsInfo.poseMsgs[18].pose.pose.orientation);
        std::cout<<"Demo OutPut Car88 -> x:"<<Car_x<<" y:"<<Car_y<<" yaw:"<<Car_yaw<<'\n';
        

        // 纯跟踪计算
        OutPut u0 = calcPurePursuit(x0, targetPos);

        cmd_vel_linear = u0.V;
        cmd_vel_angular = u0.W;


        ros::Time curTime = ros::Time::now();
        geometry_msgs::TwistStamped twist_stam;
        twist_stam.header.seq = seq++;
        twist_stam.header.stamp = curTime;
        twist_stam.header.frame_id = "base_link";

        geometry_msgs::Vector3 linear;
        linear.x = cmd_vel_linear;
        linear.y = 0;
        linear.z = 0;
        geometry_msgs::Vector3 angular;
        angular.x = 0;
        angular.y = 0;
        angular.z = cmd_vel_angular;

        if (!m_isRun)
        {
            linear.x = 0;
            angular.z = 0;
        }

        twist_stam.twist.linear = linear;
        twist_stam.twist.angular = angular;

        control_pub.publish(twist_stam);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

OutPut pure_pursuit::calcPurePursuit(const State &state, Eigen::Vector2d targetPos)
{
    OutPut u0;
    double ld = sqrt(pow(targetPos[0] - state.X, 2) + pow(targetPos[1] - state.Y, 2));
    double sita = atan2(targetPos[1] - state.Y, targetPos[0] - state.X);
    double alpha = sita - state.phi;
    if (alpha < -PI)
    {
        alpha += 2 * PI;
    }
    else if (alpha > PI)
    {
        alpha -= 2 * PI;
    }

    u0.V = currenV;
    u0.W = currenV * 2 * sin(alpha) / ld;

    u0.X = targetPos[0];
    u0.Y = targetPos[1];

    return u0;
}

void pure_pursuit::loadRoute(std::string filePath)
{
    ROS_INFO("loading route");
    std::ifstream iTrack(filePath);
    nlohmann::json jsonTrack;
    iTrack >> jsonTrack;
    std::vector<double> X = jsonTrack["X"];
    std::vector<double> Y = jsonTrack["Y"];
    Eigen::VectorXd vdX = Eigen::Map<Eigen::VectorXd>(X.data(), X.size());
    Eigen::VectorXd vdY = Eigen::Map<Eigen::VectorXd>(Y.data(), Y.size());
    track_.gen2DSpline(vdX, vdY);
}
double pure_pursuit::quaternionToYaw(const geometry_msgs::Quaternion& quat) {//四元数转角度
    // 将geometry_msgs::Quaternion转换为tf2::Quaternion
    tf2::Quaternion tf_quat;
    tf2::fromMsg(quat, tf_quat);

    // 使用tf2库来执行旋转矩阵计算
    tf2::Matrix3x3 matrix(tf_quat);

    // 获取Yaw角（绕Z轴的旋转）
    double yaw, pitch, roll;
    matrix.getRPY(roll, pitch, yaw);
    yaw+=pi/2;//改为以x轴为0方向
    if(yaw>pi) yaw=yaw-2*pi;
    return yaw;
}