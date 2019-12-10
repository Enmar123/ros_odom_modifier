#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <tf2_msgs/TFMessage.h>
#include <iostream>

ros::NodeHandle *getNodeHandle() {
  static ros::NodeHandle *nh = nullptr;
  if ( !nh )
    nh = new ros::NodeHandle("~");
  return nh;
}

void callback(const nav_msgs::Odometry msg) {
  auto nh = getNodeHandle();
  static tf::TransformBroadcaster br;

  tf::Transform transform;
  double x = msg.pose.pose.position.x;
  double y = msg.pose.pose.position.y;
  double z = msg.pose.pose.position.z;
  transform.setOrigin( tf::Vector3(x, y, z));

  tf::Quaternion q;
  q.setW( msg.pose.pose.orientation.w );
  q.setX( msg.pose.pose.orientation.x );
  q.setY( msg.pose.pose.orientation.y );
  q.setZ( msg.pose.pose.orientation.z );
  transform.setRotation(q);

  std::string frame_id, child_frame_id;

  nh->param("frame_id", frame_id, msg.header.frame_id);
  nh->param("child_frame_id", child_frame_id, msg.child_frame_id);

  br.sendTransform( tf::StampedTransform( transform, ros::Time::now(),
                                          frame_id, child_frame_id ) );
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "odom_to_tf");
  auto nh = getNodeHandle();

  std::string odom_topic_in;

  if (nh->getParam("odom_topic_in", odom_topic_in))
      ROS_INFO("Got param: %s", odom_topic_in.c_str());
  else
    ROS_ERROR("Failed to get param 'odom_topic_in'");

  ROS_INFO("Starting Node: odom_to_tf");
  ros::Subscriber sub = nh->subscribe<nav_msgs::Odometry>( odom_topic_in, 10, callback );

  ros::spin();

  delete nh;
  return 0;
}
