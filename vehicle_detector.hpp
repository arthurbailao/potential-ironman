#ifndef VEHICLE_DETECTOR_HPP
#define VEHICLE_DETECTOR_HPP

#include<vector>

#include <opencv2/video/background_segm.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "frame_processor.hpp"

class VehicleDetector : public FrameProcessor
{
public:
  VehicleDetector(bool debug = false);

  virtual void process(const cv::Mat& src, cv::Mat& dst);

  inline std::vector<cv::KeyPoint> keypoints() const
  {
    return this->_keypoints;
  }

private:
  cv::BackgroundSubtractorMOG2 _model;
  cv::Ptr<cv::FeatureDetector> _detector;
  std::vector<cv::KeyPoint> _keypoints;
  bool _debug;

  void fix(cv::Mat &src);
};

#endif // VEHICLE_DETECTOR_HPP
