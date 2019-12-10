#!/usr/bin/python


import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg import TransformStamped, Pose, PoseWithCovariance


class RosNode:
    def __init__(self):
        print("starting node: odom_modifier")
        rospy.init_node('odom_modifier')
        
        topic_in  = rospy.get_param('~odom_topic_in')
        topic_out = rospy.get_param('~odom_topic_out')
        
        self.define_msg_1()
        self.define_msg_2()
        
        self.sub = rospy.Subscriber(topic_in, Odometry, self.callback)
        self.pub = rospy.Publisher(topic_out, Odometry , queue_size=10)
            
        rospy.spin()
    
    def define_msg_1(self):
        self.pose_covariance =  [0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
                                 0.0, 0.1, 0.0, 0.0, 0.0, 0.0,
                                 0.0, 0.0, 1000000.0, 0.0, 0.0, 0.0,
                                 0.0, 0.0, 0.0, 100000.0, 0.0, 0.0,
                                 0.0, 0.0, 0.0, 0.0, 1000000.0, 0.0,
                                 0.0, 0.0, 0.0, 0.0, 0.0, 0.1]
                                         
        self.twist_covariance = [0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
                                 0.0, 1000000.0, 0.0, 0.0, 0.0, 0.0,
                                 0.0, 0.0, 1000000.0, 0.0, 0.0, 0.0,
                                 0.0, 0.0, 0.0, 1000000.0, 0.0, 0.0,
                                 0.0, 0.0, 0.0, 0.0, 1000000.0, 0.0,
                                 0.0, 0.0, 0.0, 0.0, 0.0, 0.1]
                                           
    def define_msg_2(self):
        # TODO
        self.tf_msg = TransformStamped()
        
   
    def make_tf(self):
        # TODO
        pass
        
    def callback(self, msg):
        msg.pose.covariance = self.pose_covariance
        msg.twist.covariance = self.twist_covariance
        
        self.pub.publish(msg)
        

if __name__ == '__main__':
    try:
        node = RosNode()
    except rospy.ROSInterruptException:
        pass 