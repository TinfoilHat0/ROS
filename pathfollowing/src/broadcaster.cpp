/*
 * broadcaster.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: mustafa
 */
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include <geometry_msgs/Twist.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include "arm_navigation_msgs/MultiDOFJointTrajectory.h"
#include <actionlib/client/simple_action_client.h>
using namespace std;

ros::Publisher velPublisher;
ros::Subscriber scanSubscriber;

struct Target {
	double x;
	double y;
	double rotation;
};
void chatterCallback(
		const arm_navigation_msgs::MultiDOFJointTrajectory::ConstPtr& path) {
	Target target;
	geometry_msgs::Twist robotVel;

	target.x = path->points[0].poses[0].position.x;
	target.y = path->points[0].poses[0].position.y;
	target.rotation = path->points[0].poses[0].orientation.z;
	//tf broadcaster for targets
	static tf::TransformBroadcaster br;
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(target.x, target.y, 0.0));
	transform.setRotation(tf::Quaternion(0, 0, target.rotation));
	br.sendTransform(
			tf::StampedTransform(transform, ros::Time::now(), "/odom",
					"target"));
}
int main(int argc, char **argv) {
		ros::init(argc, argv, "pathFollowing");
		ros::NodeHandle n, m;
		scanSubscriber = n.subscribe("route_cmd", 1000, chatterCallback);

		ros::spin();

		return 0;
	}

