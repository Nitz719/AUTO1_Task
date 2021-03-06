/*
*
* This program subscribes the topic ir_triggers with rosmsg IRStamped from 
* the rosbag , converts into geometry_msgs::PoseWithCovarianceStamped  
* then pusblishes into new topic ir_positions with 
* geometry_msgs::PoseWithCovarianceStamped
*
*/


/*
*
* "task1/ir_pub_sub.h" header is included contains the class that defines
* the methods for converting the rosmsgs and a callback function for the
* subscriber
*/

#include "task1/ir_pub_sub.h"

 /*
*
* Declared a (global) extern Publisher Object
*
* 
 */
 
ros::Publisher pub;

 /*
*
* main function call
*
 */
 
int main(int argc, char **argv)
{
     /*
    *
    * Create the object of the class IRpositions defined in ir_pub_sub.h 
    * and ir_pub_sub.cpp
    *
     */	
    
    IRpositions ir;

     /*
    *
    * The ros::init() function needs to see argc and argv so that it can
    * perform any ROS arguments and name remapping that were provided at
    * the command line.
    * For programmatic remappings you can use a different version of  
    * init() which takes remappings directly, but for most command-line 
    * programs, passing argc and argv is the easiest way to do it.
    *The third argument to init() is the name of the node.
    *
    * You must call one of the versions of ros::init() before using any 
    * other part of the ROS system.
    *
     */  
  
    ros::init(argc, argv, "ir_triggers");
    
     /*
    *
    * NodeHandle is the main access point to communications with the ROS 
    * system.
    * The first NodeHandle constructed will fully initialize this node, 
    * and the last
    * NodeHandle destructed will close down the node.
    *
     */
    
    ros::NodeHandle n; 

    
     /*
    * 
    * The subscribe() call is how you tell ROS that you want to receive
    * messages on a given topic.  This invokes a call to the ROS master 
    *  node, which keeps a registry of who is publishing and who
    * is subscribing.  Messages are passed to a callback function, here
    * called chatterCallback.  subscribe() returns a Subscriber object 
    * that you must hold on to until you want to unsubscribe.  When all
    * copies of the Subscriber object go out of scope,this callback will
    * automatically be unsubscribed from this topic.   
    * 
    *
    * The second parameter to the subscribe() function is the size of the
    * message queue.  If messages are arriving faster than they are being
    *  processed, this is the number of messages that will be buffered up
    *   before beginning to throw away the oldest ones.
    * 
     */ 
     
    ros::Subscriber sub = n.subscribe("ir_triggers", 1000, &IRpositions::irTriggersCallback, &ir);
  
  
     /*
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */ 
     
    pub = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/ir_positions", 10);
 
     /*
    * 
    * ros::spin() will enter a loop, pumping callbacks.  With this
    * version, all callbacks will be called from within this thread
    * (the main one).  ros::spin() will exit when Ctrl-C is pressed,
    *  or the node is shutdown by the master.
    *
     */

    ros::spin();
   
    return 0;
    
}

