// #include "ros/ros.h"
// #include "sensor_msgs/Image.h"

// #include <iostream>

// #include <image_transport/image_transport.h>
// #include <cv_bridge/cv_bridge.h>
// #include <sensor_msgs/image_encodings.h>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/highgui/highgui.hpp>

// #include <opencv2/opencv.hpp>
// #include <opencv2/nonfree/nonfree.hpp>

// #include <opencv/cv.h>

// #include <stdio.h>
// #include "std_msgs/String.h"
// #include <opencv/highgui.h>
// #include <cvaux.h>
// #include <math.h>
// #include <cxcore.h>
// #include <highgui.h>

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>

#define WN "Kinect RGB Image"

using namespace cv_bridge;

void imageCallback (const sensor_msgs::Image::ConstPtr& img);

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/camera/rgb/image_color", 1000, imageCallback);
  cv::namedWindow(WN);// Create a window for display.

  ros::spin();

  return 0;
}

void imageCallback (const sensor_msgs::ImageConstPtr& img)
{
  cv_bridge::CvImagePtr cv_ptr;

  try {

    cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8); //enc::RGB8 also used

  } catch (cv_bridge::Exception& e) {

    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }

  cv::imshow(WN, cv_ptr->image);
  cv::waitKey(1);
  
}
