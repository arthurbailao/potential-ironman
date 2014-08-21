#include "traffic_processor.hpp"

#include "frame_processor.hpp"
#include "vehicle_detector.hpp"
#include "auto_counter.hpp"

#include <iostream>
#include <sstream>

TrafficProcessor::TrafficProcessor(std::string const& filename, FrameProcessor* ptr, bool debug):
  _filename(filename),
  _capture(filename),
  _processor(ptr),
  _delay(10),
  _stop(false),
  _frameToStop(-1),
  _debug(debug)

{
  if(!this->_capture.isOpened()) {
    std::cout << "can not open camera or video file" << std::endl;
    exit(-1);
  }

  if(this->_debug) {
    cv::namedWindow("output", CV_WINDOW_NORMAL);
  }

  this->run();
}

void TrafficProcessor::run()
{
  cv::Mat frame, output;
  int total = 0;
  std::string totalText("0");

  if(!this->isOpened()) return;

  this->_stop = false;

  cv::Mat counterMask = cv::imread(this->_filename.substr(0, this->_filename.find_last_of(".")) + "_mask.bmp", CV_LOAD_IMAGE_GRAYSCALE);

  if (counterMask.empty()) {
    std::cout << "can not open counter mask file" << std::endl;
    exit(-1);
  }

  AutoCounter counter(counterMask);

  double totalFrames = this->getFrameCount();

  while(!this->isStopped()) {

    std::cout << "Progress: " << this->getFrameNumber()/totalFrames*100.0 << "%\r" << std::flush;

    if(!this->readNextFrame(frame))
      break;

    //debug
    cv::imwrite("frame.png", frame);

    if(this->_processor) {
      this->_processor->process(frame, output);

      int vehicles = counter(dynamic_cast<VehicleDetector*>(this->_processor)->keypoints());

      std::stringstream extractor;
      if(vehicles) {
        total += vehicles;
        extractor << total;
        totalText = extractor.str();
      }


      if(this->_debug) {
        cv::Mat colorMask;
        cv::cvtColor(counterMask, colorMask, CV_GRAY2BGR);
        output = output*0.8 + colorMask*0.2;
        counter.drawTrackers(output);
        cv::putText(output, totalText, cv::Point(50,50), CV_FONT_ITALIC, 2.0, cv::Scalar(0,0,255), 5, CV_AA);

        //debug
        cv::imwrite("trackers.png", output);

      }
    }

    else
      output = frame;

    if(this->_delay >= 0 && this->_debug) {
      cv::imshow("output", output);
      char k = (char)cv::waitKey(this->_delay);
//            char k = (char)cv::waitKey();
      if( k == 27 ) break;
    }

    if(this->_frameToStop >= 0 && this->getFrameNumber() == this->_frameToStop)
      this->stopIt();
  }

  //imprime o resultado
  std::cout << std::endl << "Total = " << total << std::endl;
}

void TrafficProcessor::stopIt()
{
  this->_stop = true;
}

bool TrafficProcessor::isStopped() const
{
  return this->_stop;
}

bool TrafficProcessor::isOpened() const
{
  return this->_capture.isOpened();
}

void TrafficProcessor::setDelay(int delay)
{
  this->_delay = delay;
}

void TrafficProcessor::stopAtFrame(long frame)
{
  this->_frameToStop = frame;
}

double TrafficProcessor::getFrameNumber()
{
  return static_cast<double>(this->_capture.get(CV_CAP_PROP_POS_FRAMES));
}

double TrafficProcessor::getFrameCount()
{
  return static_cast<double>(this->_capture.get(CV_CAP_PROP_FRAME_COUNT));
}

int TrafficProcessor::getFrameRate()
{
  return static_cast<int>(this->_capture.get(CV_CAP_PROP_FPS));
}

bool TrafficProcessor::readNextFrame(cv::Mat &frame)
{
  return this->_capture.read(frame);
}


