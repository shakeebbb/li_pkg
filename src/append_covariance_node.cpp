
#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <sensor_msgs/Imu.h>

void broadcastTransforms();
void imuCb(const sensor_msgs::Imu&);

ros::Publisher pub;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "append_covariance_node");
    
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("imu_raw", 10, &imuCb);
  pub = nh.advertise<sensor_msgs::Imu>("imu_topic", 10);

	ros::Rate loop_rate(10); // Publish transforms at 10Hz
	
	while(ros::ok())
		{
		//broadcastTransforms();
		ros::spinOnce();
		//loop_rate.sleep();
	  }
	  
  return 0;
};


////////////////////////////////////////////////////////
void imuCb(const sensor_msgs::Imu& inputMsg)
{
ros::NodeHandle nh;

sensor_msgs::Imu msg = inputMsg;

msg.orientation_covariance[0] = 0.01;
msg.orientation_covariance[4] = 0.01;
msg.orientation_covariance[8] = 0.01;

msg.angular_velocity_covariance[0] = 0.01;
msg.angular_velocity_covariance[4] = 0.01;
msg.angular_velocity_covariance[8] = 0.01;

msg.linear_acceleration_covariance[0] = 0.01;
msg.linear_acceleration_covariance[4] = 0.01;
msg.linear_acceleration_covariance[8] = 0.01;

pub.publish(msg);
}

void broadcastTransforms()
{
 // static tf2_ros::TransformBroadcaster br;
 // geometry_msgs::TransformStamped transformStamped;
  
 // transformStamped.header.stamp = ros::Time::now();
 // transformStamped.header.frame_id = "world";
 // transformStamped.child_frame_id = turtle_name;
 // transformStamped.transform.translation.x = msg->x;
 // transformStamped.transform.translation.y = msg->y;
 // transformStamped.transform.translation.z = 0.0;
 // tf2::Quaternion q;
 // q.setRPY(0, 0, msg->theta);
 // transformStamped.transform.rotation.x = q.x();
 // transformStamped.transform.rotation.y = q.y();
 // transformStamped.transform.rotation.z = q.z();
 // transformStamped.transform.rotation.w = q.w();

 // br.sendTransform(transformStamped);
}
