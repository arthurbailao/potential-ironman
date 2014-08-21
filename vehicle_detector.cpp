#include "vehicle_detector.hpp"

VehicleDetector::VehicleDetector(bool debug) :
  _model(),
  _detector(NULL),
  _keypoints(),
  _debug(debug)
{
  cv::SimpleBlobDetector::Params params;
//    params.minDistBetweenBlobs = 50.0f;
  params.filterByInertia = false;
  params.filterByConvexity = false;
  params.filterByColor = true;
  params.blobColor = 255;
  params.filterByCircularity = false;
  params.filterByArea = true;
  params.minArea = 500.0f;
  params.maxArea = 80000.0f;

  this->_detector = new cv::SimpleBlobDetector(params);
  this->_detector->create("SimpleBlob");

  if(debug) {
    cv::namedWindow("filter", CV_WINDOW_NORMAL);
    cv::namedWindow("foreground_mask", CV_WINDOW_NORMAL);
    cv::namedWindow("keypoints", CV_WINDOW_NORMAL);
  }

}

void VehicleDetector::process(const cv::Mat& src, cv::Mat& dst)
{
  cv::Mat gray;
  cv::cvtColor(src, gray, CV_BGR2GRAY);

  //modelador salva imagem

  cv::GaussianBlur(gray, gray, cv::Size(7, 7), 3);

  //debug
  cv::imwrite("gray.png", gray);

  cv::Mat mask;
  this->_model(gray, mask);

  //debug
  cv::imwrite("foreground.png", mask);

  fix(mask);

  //debug
  cv::imwrite("bin.png", mask);

  cv::Mat morph;

//    cv::erode(mask, morph, cv::Mat(), cv::Point(-1,-1), 2);
//    cv::medianBlur(mask, morph, 7);
//    cv::GaussianBlur(mask, morph, cv::Size(7, 7), 3);

  cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5));
  cv::morphologyEx(mask, morph, CV_MOP_CLOSE, element, cv::Point(-1,-1), 3);

  //debug
  cv::imwrite("morph.png", morph);

  this->_detector->detect(morph, this->_keypoints);

  cv::drawKeypoints(src, this->_keypoints, dst, cv::Scalar(0,0,255));

  if(this->_debug) {
    cv::Mat keypointsImg;
    cv::drawKeypoints(morph, this->_keypoints, keypointsImg, cv::Scalar(0,0,255));

    cv::imshow("filter", gray);
    cv::imshow("foreground_mask", mask);
    cv::imshow("keypoints", keypointsImg);

    //debug
    cv::imwrite("keypoints.png", keypointsImg);
  }

}

void VehicleDetector::fix(cv::Mat &src)
{
  //conta os pixeis cinza
  int sumGray = 0, sumWhite = 0;
  for(int i = 0; i < src.rows; i++) {
    const uchar* ptri = src.ptr<uchar>(i);
    for(int j = 0; j < src.cols; j++) {
      if(ptri[j] != 0 && ptri[j] != 255)
        sumGray++;
      else if(ptri[j] == 255)
        sumWhite++;
    }
  }

  if(sumWhite == 0) {
    src.setTo(cv::Scalar(0));
  }
  else if((float)sumGray/(float)sumWhite > 10.0)
    cv::threshold(src, src, 250, 255, CV_THRESH_BINARY);
  else
    cv::threshold(src, src, 5, 255, CV_THRESH_BINARY);
}
