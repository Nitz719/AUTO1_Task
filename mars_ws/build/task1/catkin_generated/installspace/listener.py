#!/usr/bin/env python3

import rospy
#from std_msgs.msg import Int64
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PointStamped


msg_header = None
msg_point = None

pub = None

def callback_number(msg):
    global msg_header,msg_point
    msg_header = msg.header
    msg_point = msg.pose.pose.position
    new_msg = PointStamped() 
    new_msg.header = msg_header
    new_msg.point = msg_point
    rospy.loginfo('The new point is %s', new_msg.point)
    pub.publish(new_msg)
   
if __name__ == '__main__':
    rospy.init_node('sensor_fusion') 
    pub = rospy.Publisher("/car_position", PointStamped, queue_size=10)
    sub = rospy.Subscriber("/odometry/filtered", Odometry, callback_number)

    
    rospy.spin()
