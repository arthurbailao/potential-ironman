#ifndef AUTO_COUNTER_HPP
#define AUTO_COUNTER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <list>
#include <vector>

#include "tracker.hpp"

class AutoCounter
{
public:
  AutoCounter(const cv::Mat& mask = cv::Mat());

  int count(const std::vector<cv::KeyPoint>& keypoints);

  //Funçao de debug: desenha trackers
  void drawTrackers(cv::Mat& img) const;
private:
  cv::Mat _mask;
  std::list<Tracker> _trackers;

  //Cria ou atualiza tracker de um keypoint
  int track(const cv::KeyPoint& keypoint);

  //Retorna trackers de um keypoint, caso existam
  std::list<Tracker*> keypointTrackers(const cv::KeyPoint& keypoint);

  //Apaga trackers sem keypoints
  void eraseDummyTrackers(const std::vector<cv::KeyPoint>& keypoints);

  bool isInCountArea(const cv::Point2f& point) const;


};

#endif // AUTO_COUNTER_HPP
