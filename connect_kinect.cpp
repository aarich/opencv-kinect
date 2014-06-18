#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>

#define WN "Kinect RGB Image"

using namespace cv;
using namespace std;

Mat img_1;

// For Keypoint Detection
int minHessian = 400;
SiftFeatureDetector detector(minHessian);
std::vector<cv::KeyPoint> keypoints1;
std::vector<cv::KeyPoint> keypoints2;

// For Descriptors
SiftDescriptorExtractor extractor;
Mat descriptors1;
Mat descriptors2;

// For Matching
FlannBasedMatcher matcher;
std::vector<cv::DMatch> matches;

void imageCallback (const sensor_msgs::ImageConstPtr& img)
{
  cv_bridge::CvImagePtr im2;

  try {

    im2 = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8); //enc::RGB8 also used

  } catch (cv_bridge::Exception& e) {

    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }

  // Detect Keypoints with SURF
  detector.detect(im2->image, keypoints2);
  // Calculate Descriptors
  extractor.compute(im2->image, keypoints2, descriptors2);
  // Match Descriptors
  matcher.match(descriptors1, descriptors2, matches);

  double max_dist = 0;
  double min_dist = 100;

  for (int i = 0; i<descriptors1.rows; i++)
  {
    double dist = matches[i].distance;
    if(dist < min_dist) min_dist = dist;
    if(dist > max_dist) max_dist = dist;
  }

  cout << "Min: " << min_dist << ", Max: " << max_dist << "." << endl;

  vector<DMatch> good_matches;

  for(int i = 0; i< descriptors1.rows; i++)
  {
    if (matches[i].distance <= max(2*min_dist, 0.02))
      good_matches.push_back( matches[i]); 
  }

  Mat img_matches;

  drawMatches( img_1, keypoints1, im2->image, keypoints2,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

  imshow (WN, img_matches);

  waitKey(1);
}

void printUsage(const char* programName)
{
  std::cout << "Type " << programName << " and an image name, e.g." << std::endl;
  std::cout << programName << " im1.png" << std::endl;
}

int main(int argc, char **argv)
{

  if (argc < 2)
  {
    printUsage(argv[0]);
    return -1;
  }
  img_1 = cv::imread(argv[1]);
  if (!img_1.data) {
    std::cout << "Error loading file.";
    return -1;
  }

  // Detect Keypoints with SURF
  detector.detect(img_1, keypoints1);

  // Calculate Descriptors
  extractor.compute(img_1, keypoints1, descriptors1);

  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/camera/rgb/image_color", 10, imageCallback);
  cv::namedWindow(WN);// Create a window for display.

  ros::spin();

  return 0;
}