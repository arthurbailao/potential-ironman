#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/core/core.hpp>

class FrameProcessor
{
public:
  virtual void process(cv::Mat const& src, cv::Mat& dst) = 0;
};

#endif // FRAME_PROCESSOR_HPP
