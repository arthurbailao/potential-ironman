#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/core/core.hpp>

#include <math.h>

namespace utils {

  float distance(const cv::Point2f& pt1, const cv::Point2f& pt2)
  {
    return sqrt(pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2));
  }
}

#endif // UTILS_HPP
