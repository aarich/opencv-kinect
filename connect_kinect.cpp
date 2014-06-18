#include "ros/ros.h"
#include "sensor_msgs/Image.h"

#include <iostream>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>

#include <opencv/cv.h>


void imageCallback (const sensor_msgs::Image::ConstPtr& img);

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/camera/rgb/image_color", 1000, imageCallback);

  ros::spin();

  return 0;
}

void imageCallback (const sensor_msgs::Image::ConstPtr& img)
{
	std::cout << "Received image" << std::endl;
  cv_bridge::CvImagePtr cv_ptr;

  try {

        cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8); //enc::RGB8 also used

    } catch (cv_bridge::Exception& e) {

        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::imshow("Kinect RGB image", cv_ptr->image);
}