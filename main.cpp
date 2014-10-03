#include <iostream>

#include <opencv2/core/core.hpp>

#include "traffic_processor.hpp"
#include "vehicle_detector.hpp"

const char* keys =
  "{h | help | false | Help}"
  "{d | debug | false | show debug outputs }"
  "{i | input |  | input file name}"
  ;

int main(int argc, char** argv)
{
  cv::CommandLineParser parser(argc, argv, keys);

  if(parser.get<bool>("help")) {
    parser.printParams();
    return 1;
  }

  bool debug = parser.get<bool>("debug");
  std::string filename = parser.get<std::string>("input");

  if(filename.empty()) {
    parser.printParams();
    return 1;
  }

  VehicleDetector detector(debug);
  TrafficProcessor processor(filename, &detector, debug);

  return 0;
}
