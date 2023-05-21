cmake_minimum_required(VERSION 2.8.3)
project( art_driver )

find_package(catkin REQUIRED COMPONENTS
  roscpp
  serial
  std_msgs
  tf
  nav_msgs
)

catkin_package(
  CATKIN_DEPENDS
    serial
    std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable( art_driver_node src/art_racecar.cpp)

target_link_libraries( art_driver_node
  ${catkin_LIBRARIES}
)
