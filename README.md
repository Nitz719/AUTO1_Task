# AUTO1_Task
Task 1 Assignment

The package is in the below git location

git location: https://github.com/Nitz719/AUTO1_Task.git

Compiling the package
-----------------------

* Open the Terminal in the mars_ws directory.
* Start the ros master by the command below
	$ roscore
* Run the following command for the compilation
	$ catkin_make

Run the code
-----------------------

* First source the workspace in the terminal at the file directory mars_ws by the command
	$ source devel/setup.bash
* Run the code by the following command
	$ roslaunch task1 task1.launch
* Added EKF Localization python code for Sensor Fusion.Run the code by the
  following command
	$ roslaunch task1 task1_ekf.launch
  ISSUE:
  	Not able to capture the first values from the bag for sensor fusion so that the calculation are going wrong due to junk data.
  CAUSE:
  	One of the cause may be the timestamps of the topics are not synchronously captured.
  	Frequency at which captured to be wrong. 
  STATUS:
  	Not Resolved
	
