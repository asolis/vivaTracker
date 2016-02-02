/**************************************************************************************************
 **************************************************************************************************
 
     BSD 3-Clause License (https://www.tldrlegal.com/l/bsd3)
     
     Copyright (c) 2016 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.
     
     
     Redistribution and use in source and binary forms, with or without modification,
     are permitted provided that the following conditions are met:
     
     1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
     3. Neither the name of the copyright holder nor the names of its contributors
        may be used to endorse or promote products derived from this software
        without specific prior written permission.
     
     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
     IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
     OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.
 
 **************************************************************************************************
 **************************************************************************************************/

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

