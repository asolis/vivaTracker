/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/
#ifndef __VivaTracker__ncc__
#define __VivaTracker__ncc__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include "tracker.h"

class NCCTracker :	public Tracker
{
public:
    virtual void initialize(const cv::Mat &img, const cv::Rect &rect);
    virtual void processFrame(const cv::Mat &img);
   	virtual void getTrackedArea(vector<Point2f> &pts);
    virtual string getDescription();
    
private:
    cv::Point2f p_position;
    cv::Size p_size;
    float p_window;
    cv::Mat p_template;
};


#endif
