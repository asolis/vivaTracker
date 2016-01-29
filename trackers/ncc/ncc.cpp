/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "ncc.h"


void NCCTracker::initialize(const cv::Mat &img, const cv::Rect &rect)
{
    p_window = MAX(rect.width, rect.height) * 2;

    int left = MAX(rect.x, 0);
    int top = MAX(rect.y, 0);

    int right = MIN(rect.x + rect.width, img.cols - 1);
    int bottom = MIN(rect.y + rect.height, img.rows - 1);

    cv::Rect roi(left, top, right - left, bottom - top);

    img(roi).copyTo(p_template);

    p_position.x = (float)rect.x + (float)rect.width / 2;
    p_position.y = (float)rect.y + (float)rect.height / 2;

    p_size = cv::Size2f(rect.width, rect.height);

}
void NCCTracker::processFrame(const cv::Mat &img)
{

    float left  = MAX(round(p_position.x - (float)p_window / 2), 0);
    float top   = MAX(round(p_position.y - (float)p_window / 2), 0);

    float right  = MIN(round(p_position.x + (float)p_window / 2), img.cols - 1);
    float bottom = MIN(round(p_position.y + (float)p_window / 2), img.rows - 1);

    cv::Rect roi((int) left, (int) top, (int) (right - left), (int) (bottom - top));

    if (roi.width < p_template.cols || roi.height < p_template.rows) {
        cv::Rect result;

        result.x = p_position.x - p_size.width / 2;
        result.y = p_position.y - p_size.height / 2;
        result.width = p_size.width;
        result.height = p_size.height;
    }

    cv::Mat matches;
    cv::Mat cut = img(roi);

    cv::matchTemplate(cut, p_template, matches, CV_TM_CCOEFF_NORMED);

    cv::Point matchLoc;
    cv::minMaxLoc(matches, NULL, NULL, NULL, &matchLoc, cv::Mat());

    p_position.x = left + matchLoc.x + (float)p_size.width / 2;
    p_position.y = top + matchLoc.y + (float)p_size.height / 2;

}

string NCCTracker::getDescription()
{
    return "ncc";
}

void NCCTracker::getTrackedArea(vector<Point2f> &pts)
{
    pts = {Point2f(p_position.x - (float)p_size.width/2.f, p_position.y - (float)p_size.height/2.f),
           Point2f(p_position.x + (float)p_size.width/2.f, p_position.y - (float)p_size.height/2.f),
           Point2f(p_position.x + (float)p_size.width/2.f, p_position.y + (float)p_size.height/2.f),
           Point2f(p_position.x - (float)p_size.width/2.f, p_position.y + (float)p_size.height/2.f)};
    
}

