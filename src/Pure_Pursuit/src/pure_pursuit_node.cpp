#include "pure_pursuit.h"

#include "ros/ros.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "pure_pursuit_node");
	ros::NodeHandle n;
	pure_pursuit node(n);

    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::waitForShutdown();

	return 0;
}