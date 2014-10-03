#ifndef TRACKER_HPP
#define TRACKER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "circle.hpp"

class Tracker
{
public:
  Tracker(const cv::KeyPoint& keypoint);

  bool contains(const cv::KeyPoint& keypoint) const;

  cv::Point2f position() const;

  void update(const cv::KeyPoint& keypoint);

  float radius() const;

  bool isDummy(const std::vector<cv::KeyPoint>& keypoints) const;

  bool operator==(const Tracker& other) const;

private:
  Circle _circle;
};

#endif // TRACKER_HPP
