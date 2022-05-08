#include "task1/ir_pub_sub.h"

 /*
*
* Macro to enable and disable the prints while testing
*
 */
 
#define test_print 0

 /*
* 
* This is a message object. You stuff it with data, and then publish it.
*
 */
 
geometry_msgs::PoseWithCovarianceStamped conv_msg;

 /*
*
* Definition for the Constructor of the class
* 
 */

IRpositions::IRpositions()
  {
  	std::cout << "Inside the Constructor " << std::endl;
  }
 
/*
*
* Definition for the Destructor of the class
* 
*/  

IRpositions::~IRpositions()
  {
  	std::cout << "Inside the Destructor " << std::endl;
  }
  
 /*
*
* The method Definition for the Subscriber callback Function of the class
* IRpositions
*
 */   

void IRpositions:: irTriggersCallback(const task1::IRStamped& msg)
   {
 
#if test_print
   
     ROS_INFO("Callback msg beam_index value: %i", msg.beam_index);
     
#endif
     
      /*
     * 
     * The convert_msg method is called to convert the custom msg
     * IRStamped into geometry_msgs::PoseWithCovarianceStamped
     *
     */
     
     conv_msg = convert_msg(msg);
     
      /*
     * 
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the 
     * type given as a template parameter to the advertise<>() call, as was
     * done in the constructor above.
     * 
      */
      
     pub.publish(conv_msg);
   }

 
 /*
* 
* The method definition to convert the custom msg IRStamped into
* geometry_msgs::PoseWithCovarianceStamped of the class IRpositions
*
 */ 
  
geometry_msgs::PoseWithCovarianceStamped& IRpositions::convert_msg(task1::IRStamped msg) 
  {
    conv_msg.header = msg.header;
    
     /*
    * 
    * Conditional statement to convert the beam_index to distance covereed 
    * in x direction.
    *
     */
     
    if(msg.beam_index == 0)
    {
       conv_msg.pose.pose.position.x = 0.1;
    }
    else if(msg.beam_index == 1)
    {
    	conv_msg.pose.pose.position.x = 0.5;
    }
    else if(msg.beam_index == 2) 
    {
    	conv_msg.pose.pose.position.x = 1.8;
    }
    else
    {
    	conv_msg.pose.pose.position.x = 1.9;
    }
            			
 #if test_print
   
     ROS_INFO("Converted msg position x : %f", conv_msg.pose.pose.position.x);
 
 # endif     
    
     /*
    * 
    * return the ros msg geometry_msgs::PoseWithCovarianceStamped to be 
    * published 
    *
     */  
         	
    return(conv_msg);
  
  }
  
        
