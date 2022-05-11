#! /usr/bin/env python
"""
Main program to perform EKF Localization

"""
import sys
sys.path.append('/home/nitz/git_workspace/AUTO1_Task/mars_ws/src/task1/src')
import rospy
import ekf_lib



# Initialisation of the ROS node
rospy.init_node('sensor_fusion')
frequency = 10.00 
Rate = rospy.Rate(frequency)  

# x is of the form [x, y, theta, v, omega]'---------
x = [[0.0], [0.0], [0.0], [0.0], [0.0]]

# P is covariance size(x) x size(x)
P = [0.0, 0.0, 0.0, 0.0, 0.0]

# variance of process noise
sigma_v = 0.1
sigma_omega = 0.1

kalman = ekf_lib.kalman_class(x, P)

if __name__ == "__main__":
    try:
        old_time = rospy.Time().now().to_sec()
        
        # loop continues while ros is not shutdown
        while not rospy.is_shutdown():

            # time step for prediction
            new_time = rospy.Time().now().to_sec()
            T = new_time - old_time

            # Estimation, estimates and return estimation error.
            # but we don't estimate the first time
            error = kalman.estimate()
            
            # predict the next robot position
            kalman.predict(T, sigma_v, sigma_omega)

            # Publish on /car_position
            kalman.publish_message()
            old_time = new_time

            # rospy.sleep(10)
            Rate.sleep()

    except rospy.ROSInterruptException:
        pass
