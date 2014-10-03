#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <opencv2/core/core.hpp>

class Circle
{
public:
  Circle(const cv::Point2f& center, float radius);

  bool contains(const cv::Point2f& pt) const;

  cv::Point2f center() const;
  void center(const cv::Point2f& center);

  float radius() const;
  void radius(float radius);

private:
  cv::Point2f _center;
  float _radius;
};

#endif // CIRCLE_HPP
