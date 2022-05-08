#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "task1/IRStamped.h"

 /*
*
* Publisher object declared as global varaible for both the main and
* ir_pub_sub cpp files
* 
 */ 
        
extern ros::Publisher pub; 
 
 /*
*
* class contains the convert msg and callback function methods
*
 */
       
class IRpositions{
        
      private :
        	
      public :
       
       /*
      *
      * Declaration for the Constructor of the class
      * 
       */ 
               
      IRpositions();
 
        /*
      *
      * Declaration for the Destructor of the class
      * 
       */
        
      ~IRpositions();
      
       /*
      *
      * The method Definition for the Subscriber callback Function of the
      * class IRpositions
      * 
       */  
        
      void irTriggersCallback(const task1::IRStamped& msg);
      
       /*
      * 
      * The method definition to convert the custom msg IRStamped into
      * geometry_msgs::PoseWithCovarianceStamped of the class IRpositions
      *
       */
        
      geometry_msgs::PoseWithCovarianceStamped& convert_msg(task1::IRStamped msg) ;
   
        };
