cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
 
project(connect_kinect)

find_package( catkin REQUIRED COMPONENTS roscpp )
find_package( OpenCV REQUIRED )
find_package(Boost REQUIRED)

add_executable(connect_kinect connect_kinect.cpp)

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})

target_link_libraries (connect_kinect ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} /opt/ros/hydro/lib/libcv_bridge.so)
