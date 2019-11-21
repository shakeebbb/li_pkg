
#include <ros/ros.h>
#include <tf/tf.h>
#include <tf2/LinearMath/Quaternion.h>
#include <sensor_msgs/Imu.h>
#include <math.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

void rangeCb(const sensor_msgs::Range&);
void imuCb(const sensor_msgs::Imu&);

float currentRoll = 0;
float currentPitch = 0;
float currentYaw = 0;

ros::Publisher pub;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "range2pose");
    
  ros::NodeHandle nh;
  ros::Subscriber rangeSub = nh.subscribe("range_topic", 10, &rangeCb);
	ros::Subscriber imuSub = nh.subscribe("imu_topic", 10, &imuCb);

  pub = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("pose_topic", 10);

	ros::spin();

  return 0;
};


////////////////////////////////////////////////////////

void imuCb(const sensor_msgs::Imu& imuMsg)
{

tf::Quaternion q(
        imuMsg.orientation.x,
        imuMsg.orientation.y,
        imuMsg.orientation.z,
        imuMsg.orientation.w);

tf::Matrix3x3 m(q);
double roll, pitch, yaw;
m.getRPY(roll, pitch, yaw);

currentRoll = roll;
currentPitch = pitch;
currentYaw = yaw;
}

void rangeCb(const sensor_msgs::Range& rangeMsg)
{

geometry_msgs::PoseWithCovarianceStamped poseMsg;

poseMsg.header = rangeMsg.header;

poseMsg.pose.pose.position.x = 0;
poseMsg.pose.pose.position.y = 0;

static float prev_range = 0;

if (isinf(rangeMsg.range))
{
	poseMsg.pose.pose.position.z = prev_range;
	poseMsg.pose.covariance = {0,0,0,0,0,0,
																	0,0,0,0,0,0,
																	0,0,0.01,0,0,0,
																	0,0,0,0,0,0,
																	0,0,0,0,0,0,
																	0,0,0,0,0,0};
}
else
{
	poseMsg.pose.pose.position.z = rangeMsg.range*cos(currentRoll)*cos(currentPitch);
	poseMsg.pose.covariance = {0,0,0,0,0,0,
																	0,0,0,0,0,0,
																	0,0,0.004,0,0,0,
																	0,0,0,0,0,0,
																	0,0,0,0,0,0,
																	0,0,0,0,0,0};
}

prev_range = poseMsg.pose.pose.position.z;

poseMsg.pose.pose.orientation.x = 0;
poseMsg.pose.pose.orientation.y = 0;
poseMsg.pose.pose.orientation.z = 0;
poseMsg.pose.pose.orientation.w = 1;

pub.publish(poseMsg);
}
