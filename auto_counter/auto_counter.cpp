#include "auto_counter.hpp"

#include <iostream>

#include <opencv2/highgui/highgui.hpp>

namespace {
  float distance(const cv::Point2f& pt1, const cv::Point2f& pt2)
  {
    return sqrt(pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2));
  }
}

AutoCounter::AutoCounter(const cv::Mat& mask)
  : _mask(mask)
  , _trackers()
{
}

int AutoCounter::count(const std::vector<cv::KeyPoint>& keypoints)
{
  int total = 0;
  std::vector<cv::KeyPoint>::const_iterator keypoint = keypoints.begin();
  for(; keypoint != keypoints.end(); ++keypoint) {
    total += this->track(*keypoint);
  }

  this->eraseDummyTrackers(keypoints);
  return total;
}

int AutoCounter::track(const cv::KeyPoint& keypoint)
{
  std::list<Tracker*> tracked = this->keypointTrackers(keypoint);
  if(tracked.empty()) {
    if(!this->isInCountArea(keypoint.pt))
      this->_trackers.push_back(Tracker(keypoint));
  } else if(tracked.size() == 1) {
    if((this->isInCountArea(keypoint.pt)) && (!this->isInCountArea(tracked.front()->position()))) {
      this->_trackers.remove(*tracked.front());
      return 1;
    } else {
      tracked.front()->update(keypoint);
    }
  } else {
    //pega o tracker mais proximo do keypoint
    float maxDistance = FLT_MAX;
    Tracker* bestTracker = NULL;
    for(std::list<Tracker*>::iterator it = tracked.begin(); it != tracked.end(); ++it) {
      if(float currentDistance = distance(keypoint.pt, (*it)->position()) < maxDistance) {
        if(bestTracker)
          this->_trackers.remove(*bestTracker);
        maxDistance = currentDistance;
        bestTracker = (*it);
      }
    }

    if(this->isInCountArea(keypoint.pt) && !this->isInCountArea(bestTracker->position())) {
      this->_trackers.remove(*bestTracker);
      return 1;
    } else {
      bestTracker->update(keypoint);
    }
  }

  return 0;
}

std::list<Tracker*> AutoCounter::keypointTrackers(const cv::KeyPoint& keypoint)
{
  std::list<Tracker*> tracked;
  std::list<Tracker>::iterator tracker = this->_trackers.begin();
  for(; tracker != _trackers.end(); ++tracker) {
    if(tracker->contains(keypoint))
      tracked.push_back(&(*tracker));
  }
  return tracked;
}

void AutoCounter::eraseDummyTrackers(const std::vector<cv::KeyPoint>& keypoints)
{
  std::list<Tracker>::iterator tracker = this->_trackers.begin();
  while(tracker != _trackers.end()) {
    if(tracker->isDummy(keypoints))
      tracker = this->_trackers.erase(tracker);
    else
      ++tracker;
  }
}

bool AutoCounter::isInCountArea(const cv::Point2f& point) const
{
  if(this->_mask.empty())
    return false;

  double intpart;
  double x = modf(point.x, &intpart) >= 0.5f ? ceil(point.x) : floor(point.x);
  double y = modf(point.y, &intpart) >= 0.5f ? ceil(point.y) : floor(point.y);
  return (int)this->_mask.at<uchar>(cv::Point((int)x, (int)y)) > 0;
}

void AutoCounter::drawTrackers(cv::Mat& img) const
{
  if(img.empty())
    return;

  std::list<Tracker>::const_iterator tracker = this->_trackers.begin();
  for(; tracker != _trackers.end(); ++tracker) {
    cv::circle(img, tracker->position(), tracker->radius(), cv::Scalar(255,0,0), 2);
  }
}
