
#include "ros/ros.h"
#include "mav_planning_msgs/PlannerService.h"
#include "std_srvs/Empty.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "voxblox_planner_node");

  ros::NodeHandle nh;

  ros::ServiceClient planClient = nh.serviceClient<mav_planning_msgs::PlannerService>("voxblox_rrt_planner/plan");
  ros::ServiceClient publishPathclient = nh.serviceClient<std_srvs::Empty>("voxblox_rrt_planner/publish_path");

  mav_planning_msgs::PlannerService planSrv;

  planSrv.request.goal_pose.header.stamp = ros::Time::now();
	planSrv.request.goal_pose.pose.position.x = 0;
	planSrv.request.goal_pose.pose.position.y = 5;
	planSrv.request.goal_pose.pose.position.z = 0.5;

	ros::Rate loop_rate(0.5);
	while(ros::ok())
	{
		if (planClient.call(planSrv) && planSrv.response.success)
		{
			std_srvs::Empty publishPathSrv;
			if(publishPathclient.call(publishPathSrv))
			ROS_INFO("Successfully Published Path");
			else
			ROS_ERROR("Unable to call publish_path service");
		}
		else
		ROS_ERROR("Failed to call service plan");

		loop_rate.sleep();
	}

  return 0;
}
