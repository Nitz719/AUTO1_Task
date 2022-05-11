

"""
Program that has all the class and methods to perform EKF Localization

"""

import numpy as np
import rospy
from scipy.linalg import block_diag
from tf.transformations import euler_from_quaternion, quaternion_from_euler
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
from geometry_msgs.msg import Twist, Point, Quaternion, PoseWithCovarianceStamped, TwistWithCovarianceStamped , PointStamped


def normalize_angle(angle):
    # the euler_from_quaternion gives angle in [-pi, pi]
    # So Normalize angle to [-pi, pi]"""
    
    y = angle
    y = y % (2 * np.pi)    
    if y > np.pi:             
        y -= 2 * np.pi
    return y


class kalman_class():

    def __init__(self, x, P):

        self.not_first_time = False
        self.previous_x = np.matrix(x)
        self.previous_P = np.diag(P)
        self.estimated_x = self.previous_x
        self.estimated_P = self.previous_P
        self.predicted_x = self.previous_x
        self.predicted_P = self.previous_P                          
        self.odom_covariance = np.empty((3, 3), dtype=int)
        self.odom_x = 0
        self.odom_y = 0
        self.odom_theta = 0
        self.I_see_something = False
        self.time_stamp = None
                                 
    def callback_pose(self, msg):
        
        self.time_stamp = msg.header.stamp
        self.odom_x = msg.pose.pose.position.x
        self.odom_y = msg.pose.pose.position.y
        
        # theta is the yaw we need to use tf.transforms quater -> euler
        # 
        
        # euler to quaternion conversion
        quat = msg.pose.pose.orientation
        (roll, pitch, yaw) = euler_from_quaternion((quat.x, quat.y, quat.z, quat.w))
        
        self.odom_theta = yaw
        pose_covariance = msg.pose.covariance
        self.odom_covariance = np.diag([pose_covariance[0],
                                            pose_covariance[0],
                                            pose_covariance[0]])  
                                            
    def callback_velocity(self, msg):
        # calculates velocity and updates x
        vx = msg.twist.twist.linear.x
        vy = msg.twist.twist.linear.y
        omega = msg.twist.twist.angular.z
        v = np.sqrt(vx**2 + vy**2)
        self.estimated_x[3, 0] = v
        self.estimated_x[4, 0] = omega
                                                     
            
    def update_pose(self):
        # subscriber in ir topic
        rospy.Subscriber('/ir_positions', PoseWithCovarianceStamped, self.callback_pose)
        
    def update_velocity(self):
        # subscriber in radar topic
        rospy.Subscriber('/radar_samples', TwistWithCovarianceStamped, self.callback_velocity)
 
# Predicted step of the pose utilizing the /radar_sample and
# estimated_pose
        
    def predict(self, T, sigma_v, sigma_omega):
        """
        f = Matrix([[x + v * T * cos(theta)],
                    [y + v * T * sin(theta)],
                    [theta + omega],
                    [v],
                    [omega]])
        """
        self.predicted_x[2, 0] = self.estimated_x[2, 0] + self.estimated_x[4, 0] * T
        # normalize angle [-pi pi]------------------------------------------------
        self.predicted_x[2, 0] = normalize_angle(self.predicted_x[2, 0])
        # ------------------------------------------------------------------------
        self.predicted_x[0, 0] = self.estimated_x[0, 0] + self.estimated_x[3, 0] * T * np.cos(self.predicted_x[2, 0])
        self.predicted_x[1, 0] = self.estimated_x[1, 0] + self.estimated_x[3, 0] * T * np.sin(self.predicted_x[2, 0])
        self.predicted_x[3, 0] = self.estimated_x[3, 0]
        self.predicted_x[4, 0] = self.estimated_x[4, 0]

        # d_f is the jacobian of f for u = [v, omega]'
        ang = T*self.estimated_x[4, 0] + self.estimated_x[2, 0]
        # normalize angle [-pi pi]-----------------------------------------------
        ang = normalize_angle(ang)
        # ------------------------------------------------------------------------

        d_f = np.matrix([[T*np.cos(ang), (-(T**2)*self.estimated_x[3, 0]*np.sin(ang))],
                        [T*np.sin(ang), (T**2)*self.estimated_x[3, 0]*np.cos(ang)],
                        [0, T],
                        [1, 0],
                        [0, 1]])

        # d_f_prime is the jacobian of f for x
        d_f_prime = np.matrix([[1, 0, -T*self.estimated_x[3, 0]*np.sin(ang), T*np.cos(ang), -(T**2)*self.estimated_x[3, 0]*np.sin(ang)],
                              [0, 1, T*self.estimated_x[3, 0]*np.cos(ang), T*np.sin(ang), (T**2)*self.estimated_x[3, 0]*np.cos(ang)],
                              [0, 0, 1, 0, T],
                              [0, 0, 0, 1, 0],
                              [0, 0, 0, 0, 1]])

        var_Q = np.matrix([[sigma_v**2, 0], [0, sigma_omega**2]])
        Q = d_f * var_Q * d_f.T
        self.predicted_P = d_f_prime * self.estimated_P * d_f_prime.T + Q

        # old state and covariance update
        self.previous_x = self.predicted_x
        self.previous_P = self.predicted_P

# Estimating the pose with respect to the IR_positions
 
    def estimate(self):
        # measurement in form z = Cx + v , v is white noise
        
        # Subscribing the IR topic
        self.update_pose()
         
        z = np.matrix([[self.odom_x], [self.odom_y],
                           [self.odom_theta]])
                           
        C = np.matrix([[1, 0, 0, 0, 0],
                                 [0, 1, 0, 0, 0],
                                 [0, 0, 1, 0, 0]])
                                 
 	# Was not able read the covariance value for first few values
 	# though hardcoded the value here.
 	# Inverse function will through singular matrix error though junk 
 	# values are fetched.
 	# CAUSE: Suspected to match with the timestamps of the topics
 	
 	# R = self.odom_covariance
        R = np.diag([0.0005,0.0005,0.0005])
  	            
        # Kalman gain
        S = C * self.previous_P * C.T + R
        self.K = self.previous_P * C.T * np.linalg.inv(S)

        # error calcualtion
        error = z - (C * self.previous_x)
        # normalize angle ------------------------------------------------
        error[2, 0] = normalize_angle(error[2, 0])
        # -----------------------------------------------------------------

        # we don't estimate first time
        if self.not_first_time:
            self.estimated_x = self.previous_x + self.K * error
            # normalize angle [-pi pi]------------------------------------
            self.estimated_x[2, 0] = normalize_angle(self.estimated_x[2, 0])
            # ------------------------------------------------------------
            mat = np.eye(5, dtype=int) - self.K * C
            self.estimated_P = mat * self.previous_P
        else:
            self.not_first_time = True

        self.update_velocity()
        return error
	
# Publishing the estimated pose 

    def publish_message(self):
        # publisher
        Pub = rospy.Publisher('/car_position', PointStamped, queue_size=10)
        msg_point = PointStamped()
        # Publish kalma.x and kalman.P
        x = self.estimated_x[0, 0]
        y = self.estimated_x[1, 0]

        # Has no importance
        z = 0
        
        msg_point.header.stamp = self.time_stamp
        msg_point.point = Point(x, y, z)

        # publishing the message
        Pub.publish(msg_point)




