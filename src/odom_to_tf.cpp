#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <tf2_msgs/TFMessage.h>
#include <iostream>

using namespace std;

static ros::Publisher pub;


void callback(const nav_msgs::Odometry msg){

  //geometry_msgs::TransformStamped transform;
  tf2_msgs::TFMessage transforms;

  cout << "marker 1" << endl;

  //transforms.transforms.assign(0, 0); // First and last iterator (size of array)

  transforms.transforms.begin()->header = msg.header;
  cout << "maker 1.5" << endl;
  transforms.transforms.begin()->child_frame_id = "encoder_odom";
  transforms.transforms.begin()->transform.translation.x = msg.pose.pose.position.x;
  transforms.transforms.begin()->transform.translation.y = msg.pose.pose.position.y;
  transforms.transforms.begin()->transform.translation.z = msg.pose.pose.position.z;
  transforms.transforms.begin()->transform.rotation      = msg.pose.pose.orientation;

//  transforms.transforms[0].header = msg.header;
//  transforms.transforms[0].child_frame_id = "encoder_odom";
//  transforms.transforms[0].transform.translation.x = msg.pose.pose.position.x;
//  transforms.transforms[0].transform.translation.y = msg.pose.pose.position.y;
//  transforms.transforms[0].transform.translation.z = msg.pose.pose.position.z;
//  transforms.transforms[0].transform.rotation      = msg.pose.pose.orientation;

  cout << "marker 2" << endl;

  pub.publish(transforms);
}

void callback2(const nav_msgs::Odometry msg){
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

  br.sendTransform( tf::StampedTransform( transform, ros::Time::now(), "ekf_odom", "encoder_odom" ));
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "odom_to_tf");
  ros::NodeHandle nh;

  ROS_INFO("Starting Node: odom_to_tf");
  pub = nh.advertise<tf2_msgs::TFMessage>("tf", 10);
  ros::Subscriber sub = nh.subscribe<nav_msgs::Odometry>( "jackal_velocity_controller/odom", 10, callback2 );

  ros::spin();
  return 0;
}
