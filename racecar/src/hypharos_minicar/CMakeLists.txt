cmake_minimum_required(VERSION 3.5)
project(hypharos_minicar)

## Compile as C++11, supported in ROS Kinetic and newer
add_definitions(-std=c++11 -O3)

# options for build configuration
option(BUILD_EXAMPLE "Whether or not building the CppAD & Ipopt example" OFF) 

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  costmap_2d
  dynamic_reconfigure
  geometry_msgs
  nav_msgs
  ackermann_msgs
  message_generation
  move_base
  roscpp
  rospy
  std_msgs
  tf
  visualization_msgs
  actionlib
  actionlib_msgs
  move_base_msgs
)



catkin_package(
   #INCLUDE_DIRS include
#  LIBRARIES hypharos_minicar
#  CATKIN_DEPENDS costmap_2d dynamic_reconfigure geometry_msgs message_generation move_base roscpp rospy std_msgs tf visualization_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include ${catkin_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS} include )
include_directories(/usr/include)
link_directories(/usr/lib)


# Pure Pursuit Node
add_executable(Pure_Pursuit src/Pure_Pursuit.cpp)
target_link_libraries(Pure_Pursuit ${catkin_LIBRARIES})

add_executable(mini_car_controller src/mini_car_controller.cpp)
target_link_libraries(mini_car_controller ${catkin_LIBRARIES})

add_executable(send_goal src/send_goal.cpp)
target_link_libraries(send_goal ${catkin_LIBRARIES})

add_executable(many_goal src/many_goal.cpp)
target_link_libraries(many_goal ${catkin_LIBRARIES})

#############
## Example ##
#############
if(BUILD_EXAMPLE)
    ADD_EXECUTABLE( CppAD_started example/CppAD_started.cpp )
    TARGET_LINK_LIBRARIES(CppAD_started)
    
    ADD_EXECUTABLE( CppAD_Ipopt example/CppAD_Ipopt.cpp )
    TARGET_LINK_LIBRARIES(CppAD_Ipopt ipopt)
endif(BUILD_EXAMPLE)

#add_action_files(DIRECTORY action FILES DoDishes.action) generate_messages(DEPENDENCIES actionlib_msgs)





