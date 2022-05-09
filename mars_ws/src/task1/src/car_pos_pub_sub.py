#!/usr/bin/env python


"""

  This program subscribes the topic /odometry/filtered from the ekf
  localization node with rosmsg nav_msgs/Odometry converts into rosmsg
  geometry_msgs/PointStamped then pusblishes into new topic car_positions


"""

import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PointStamped

# Intailized the global variables
pub = None
msg_header = None
msg_point = None

# Definition of the Subscriber callback function

def callback_number(msg):
    global msg_header,msg_point
    
    # Copying the Odometry msg values to the global variables
    msg_header = msg.header
    msg_point = msg.pose.pose.position
    
    # Creating the PointStamped rosmsg object
    new_msg = PointStamped() 
    
    # Converting the Odometry rosmsg to PointStamped rosmsg
    new_msg.header = msg_header
    new_msg.point = msg_point
    
    # Printing the PointStamped pose values
    rospy.loginfo('The new point is %s', new_msg.point)
    
    # Publishing the PointStamped msg
    pub.publish(new_msg)
   
if __name__ == '__main__':

    # INitialization of the ros node
    rospy.init_node('sensor_fusion') 
    
    # Creating the Publisher object
    pub = rospy.Publisher("/car_position", PointStamped, queue_size=10)
    
    # Creating the Subscriber object
    sub = rospy.Subscriber("/odometry/filtered", Odometry, callback_number)

    
    rospy.spin()
