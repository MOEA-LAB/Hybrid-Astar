## 1.基本介绍
### 1.1 项目介绍
smart_car是一个基于ROS的智能车控制程序

## 2.功能包(package)介绍

**2.4和2.6功能包分别使用了串口和wifi来实现了动捕数据接收功能，使用时只需要启用其中一个即可，两种方式切换方法为修改comm_hub/launch/start.launch中启动的节点**

### 2.1 collision_avoid
ROS 智能车自动避障包
```
    ├── collision_avoid
        ├── include                         
        │   └── collision_avoid.h           (自动避障头文件)
        ├── src                             
        │   ├── collision_avoid_node.cpp    (节点源文件)
        │   └── collision_avoid.cpp         (自动避障源文件)
        ├── CMakeLists.txt                  (CMake文件)
        └── package.xml                     (信息描述文件)                     
```

### 2.2 comm_hub
ROS 智能车数据上报包（程序启动入口包）
```
    ├── comm_hub
        ├── include
        │   ├── nlohmann                        
        │   │   └── json.hpp        (第三方json库)
        │   ├── comm_hub.h          (数据上报类头文件)
        │   ├── myMsg.h             (信息封装函数头文件)
        │   └── myUdp.h             (UDP类头文件)
        ├── src                             
        │   ├── comm_hub_node.cpp   (节点源文件)
        │   ├── comm_hub.cpp        (数据上报类源文件)
        │   ├── myMsg.cpp           (节点源文件)
        │   └── myUdp.cpp           (自动避障源文件)
        ├── params
        │   └── config.yaml         (参数配置文件)
        ├── launch
·        │   └── start.launch        (程序启动launch文件)
        ├── CMakeLists.txt          (CMake文件)
        └── package.xml             (信息描述文件)                     
```

### 2.3 Pure_Pursuit
ROS 纯跟踪算法包
```
    ├── Pure_Pursuit
        ├── External
        │   └── Eigen                   (第三方库 不展开说明)  
        ├── include
        │   ├── arc_length_spline.h 
        │   ├── cubic_spline.h          
        │   ├── pure_pursuit.h          (纯跟踪算法类头文件)
        │   └── track.h           
        ├── src                             
        │   ├── arc_length_spline.cpp 
        │   ├── cubic_spline.cpp          
        │   ├── pure_pursuit_node.cpp   (节点源文件)
        │   ├── pure_pursuit.cpp        (纯跟踪算法类源文件)
        │   └── track.cpp 
        ├── routes                      (路径存储文件夹)
        ├── params
        │   └── config.yaml             (参数配置文件)
        ├── CMakeLists.txt              (CMake文件)
        └── package.xml                 (信息描述文件)                     
```

### 2.4 serial_ros
ROS 串口数据接收包
```
    ├── serial_ros
        ├── include
        │   ├── nlohmann                        
        │   │   └── json.hpp            (第三方json库)
        │   └── serial_ros.h            (串口数据接收类头文件)
        ├── src    
        │   ├── serial_ros_node.cpp     (节点源文件)
        │   └── serial_ros.cpp          (串口数据接收类源文件)                         
        ├── msg    
        │   └── PoseMsgs.msg            (自定义话题topic格式)                         
        ├── CMakeLists.txt              (CMake文件)
        └── package.xml                 (信息描述文件)                     
```

### 2.5 turn_on_qiansheng_robot
ROS 底盘控制包
**非特殊情况无需修改，故不做展开说明**

### 2.6 vrpn_client_ros-kinetic-devel
ROS VRPN客户端包(用于接收动捕定位数据)
**非特殊情况无需修改，故不做展开说明**

## 3.话题(topic)介绍
可使用**rostopic list**命令查看ros中所有话题(topic)信息(加-v显示详细信息，即**rostopic list -v**)
可使用**rostopic echo /A**命令输出ros中A话题(topic)的实时数据
其余命令请自行查找rostopic相关资料


| 话题 | 类型 | 所需头文件 | 功能 |
| ---- | ---- | ---- | ---- |
| /PowerVoltage | [std_msgs/Float32] | <std_msgs/Float32.h> | 
| /cmd_vel | [geometry_msgs/TwistStamped] | <geometry_msgs/TwistStamped.h> |
| /collision_avoid_ctrl | [std_msgs/UInt32] | <std_msgs/UInt32.h> |
| /ctrl_run | [std_msgs/Bool] | <std_msgs/Bool.h> |
| /ctrl_speed | [std_msgs/UInt32] | <std_msgs/UInt32.h> |
| /ctrl_switch_mode | [std_msgs/UInt32] | <std_msgs/UInt32.h> |
| /imu | [sensor_msgs/Imu] | <sensor_msgs/Imu.h> |
| /motion_capture_pose | [geometry_msgs/PoseWithCovarianceStamped] | <geometry_msgs/PoseWithCovarianceStamped.h> |
| /motion_capture_pose_all | [serial_ros/PoseMsgs] | "serial_ros/PoseMsgs.h" |
| /odom | [nav_msgs/Odometry] | <nav_msgs/Odometry.h> |
| /vrpn_client_node/CarX/pose | [geometry_msgs/PoseWithCovarianceStamped] | <geometry_msgs/PoseWithCovarianceStamped.h> |

## 4.简要示例
**以下代码仅供参考，请自行添加至功能包(package)后使用**

话题(topic)发布示例，用/cmd_vel作为示例
```
#include "ros/ros.h"
#include <geometry_msgs/TwistStamped.h>

int main(int argc, char **argv)
{
    int seq;
    double cmd_vel_linear;
    double cmd_vel_angular;

    ros::Publisher control_pub;
    control_pub = n.advertise<geometry_msgs::TwistStamped>("/cmd_vel", 10, true);

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

    twist_stam.twist.linear = linear;
    twist_stam.twist.angular = angular;

    control_pub.publish(twist_stam);

    return 0;
}
```

话题(topic)订阅示例，用/motion_capture_pose作为示例
```
#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <tf/transform_datatypes.h>

void _MotionCapturePoseCallback(const nav_msgs::Odometry &msg){
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg.pose.pose.orientation, quat);
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
    yaw = yaw + PI / 2 + m_angleError;

    // 朝向角
    yaw = yaw;
    // x
    auto x = msg.pose.pose.position.x;
    // y
    auto y = msg.pose.pose.position.y;
}

int main(int argc, char **argv)
{
    // 订阅名为"motion_capture_pose"的话题，最后一个参数为回调函数，当接收到新消息时会调用这个函数
    ros::Subscriber motion_capture_pose_sub;
    motion_capture_pose_sub = n.subscribe("/motion_capture_pose", 10, _MotionCapturePoseCallback);
    
    // 回调处理函数
    ros::spin();

    return 0;
}
```