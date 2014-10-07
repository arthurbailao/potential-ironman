#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

const char* keys =
  "{h | help | false | Help}"
  "{f | frames | 30 | slice of frames to play }"
  "{d | delay | 30 | delay between frames}"
  "{i | input |  | input file name}"
  ;

int main(int argc, char** argv)
{
  cv::CommandLineParser parser(argc, argv, keys);

  if(parser.get<bool>("help")) {
    parser.printParams();
    return 1;
  }

  int frames = parser.get<int>("frames");
  int delay = parser.get<int>("delay");
  std::string filename = parser.get<std::string>("input");

  if(filename.empty()) {
    parser.printParams();
    return 1;
  }

  cv::VideoCapture capture(filename);

  if(!capture.isOpened()) {
    std::cout << "can not open camera or video file" << std::endl;
    return 1;
  }

  cv::namedWindow("video", CV_WINDOW_NORMAL);
  for(;;) {
    cv::Mat frame;
    capture >> frame;

    char k = (char)cv::waitKey(delay);
    if(k == 27) break;

    int count = static_cast<int>(capture.get(CV_CAP_PROP_POS_FRAMES));
    std::stringstream extractor;
    extractor << count;

    cv::putText(frame, extractor.str(), cv::Point(frame.cols - 150, frame.rows - 10), CV_FONT_ITALIC, 1.0, cv::Scalar(0,0,255), 5, CV_AA);
    cv::imshow("video", frame);

    if(count % frames == 0) {
      while(k != 27 && k != 32) {
        k = (char)cv::waitKey();
      }
      if(k == 27) break;
    }

  }
  return 0;
}
