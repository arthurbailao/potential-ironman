#include "circle.hpp"

namespace {
  float distance(const cv::Point2f& pt1, const cv::Point2f& pt2)
  {
    return sqrt(pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2));
  }
}

Circle::Circle(const cv::Point2f& center, float radius)
  : _center(center)
  , _radius(radius)
{
}

bool Circle::contains(const cv::Point2f& pt) const
{
  return distance(this->center(), pt) < this->radius();
}

cv::Point2f Circle::center() const
{
  return this->_center;
}

void Circle::center(const cv::Point2f& center)
{
  this->_center = center;
}

float Circle::radius() const
{
  return this->_radius;
}

void Circle::radius(float radius)
{
  this->_radius = radius;
}
