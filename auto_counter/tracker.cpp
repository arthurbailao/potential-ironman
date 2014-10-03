#include "tracker.hpp"

Tracker::Tracker(const cv::KeyPoint& keypoint)
  : _circle(keypoint.pt, keypoint.size)
{
}

bool Tracker::contains(const cv::KeyPoint& keypoint) const
{
  return this->_circle.contains(keypoint.pt);
}

cv::Point2f Tracker::position() const
{
  return this->_circle.center();
}

void Tracker::update(const cv::KeyPoint& keypoint)
{
  this->_circle.center(keypoint.pt);
  this->_circle.radius(keypoint.size);
}

float Tracker::radius() const
{
  return this->_circle.radius();
}

bool Tracker::isDummy(const std::vector<cv::KeyPoint>& keypoints) const
{
  std::vector<cv::KeyPoint>::const_iterator keypoint = keypoints.begin();
  for(; keypoint != keypoints.end(); ++keypoint) {
    if(this->contains(*keypoint))
      return false;
  }
  return true;
}

bool Tracker::operator==(const Tracker& other) const
{
  return (this->position() == other.position()) && (this->radius() == other.radius());
}
