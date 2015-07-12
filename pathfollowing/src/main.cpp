#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
using namespace std;
int main(int argc, char** argv) {
	ros::init(argc, argv, "my_tf_listener");

	ros::NodeHandle node;
	ros::Publisher robot_vel = node.advertise<geometry_msgs::Twist>("cmd_vel",
			10);

	tf::TransformListener listener;

	ros::Rate rate(10.0);
	while (node.ok()) {
		tf::StampedTransform transform;
		try {

			listener.waitForTransform("/base_link", "/target", ros::Time(0),
					ros::Duration(3.0));
			listener.lookupTransform("/base_link", "/target", ros::Time(0),
					transform);
		} catch (tf::TransformException ex) {
			ROS_ERROR("%s", ex.what());
		}

		geometry_msgs::Twist vel_msg;
		vel_msg.angular.z = 0.5 * atan2(transform.getOrigin().y(), transform.getOrigin().x());
		vel_msg.linear.x = 0.2;
		cout << "Rot: " << transform.getRotation().getZ() << endl;

		robot_vel.publish(vel_msg);

		rate.sleep();
	}
	return 0;
}
;
