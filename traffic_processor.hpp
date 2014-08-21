#ifndef TRAFFIC_PROCESSOR_HPP
#define TRAFFIC_PROCESSOR_HPP

#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class FrameProcessor;

class TrafficProcessor
{
public:
  TrafficProcessor(std::string const& filename, FrameProcessor* ptr = NULL, bool debug = false);

  void setDelay(int _delay);

  void stopAtFrame(long frame);

private:
  std::string _filename;
  cv::VideoCapture _capture;
  FrameProcessor* _processor;
  int _delay;
  bool _stop;
  long _frameToStop;
  bool _debug;

  void stopIt();

  void run();

  bool readNextFrame(cv::Mat &frame);

  bool isStopped() const;

  bool isOpened() const;

  double getFrameNumber();

  double getFrameCount() ;

  int getFrameRate();


};

#endif // TRAFFIC_PROCESSOR_HPP
