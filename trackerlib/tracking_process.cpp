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


#include "tracking_process.h"




RectSelectArea::RectSelectArea():
_state(0), _loc(2, Point2f(-1,-1))
{
    _loc[0] = Point2f(-1,-1);
    _loc[1] = Point2f(-1,-1);
}
void RectSelectArea::mouseMove(int x, int y)
{
    if (_state == 1)
        _loc[1] = Point2f(x,y);
}

void RectSelectArea::setClick(int x , int y)
{
    
    if (_state == 0)
    {
        _loc[0]= Point2f(x,y);
        _loc[1]= Point2f(x,y);
    }
    else if (_state == 1)
    {
        _loc[1]= Point2f(x,y);
    }
    else if (_state == 2)
    {
        _state++;
        _loc[0]= Point2f(x,y);
        _loc[1]= Point2f(x,y);
    }
    _state = (++_state) % 3;
    
}
bool RectSelectArea::isSelected()
{
    return _state == 2;
}

Rect RectSelectArea::getBoundingBox()
{
    Rect tmp  = boundingRect(_loc);
    tmp.width -= 1;
    tmp.height -= 1;
    return tmp;
}

void TrackingProcess::setTracker(const Ptr<Tracker> &trk)
{
    tracker = trk;
}
//@Override
void TrackingProcess::leftButtonDown(int x, int y, int flags)
{
    selectedArea.setClick(x,y);
}
//@Override
void TrackingProcess::mouseMove(int x, int y, int flags)
{
    selectedArea.mouseMove(x, y);
}
//@Override
void TrackingProcess::operator()(const size_t frameN, const Mat &frame, Mat &output)
{
    frame.copyTo(output);
    
    if (frameN < groundTruth.size())
    {
        Draw::drawQuadrangle(output, groundTruth[frameN], Color::white);
    }
    if (frameN == 0 && groundTruth.size() > 0)
    {
        Rect _area_ = boundingRect(groundTruth[0]);
        _area_.width -= 1;
        _area_.height -= 1;
        selectedArea.setClick(_area_.tl().x, _area_.tl().y);
        selectedArea.setClick(_area_.br().x, _area_.br().y);
    }

    if (!selectedArea.isSelected())
    {
        trackerInitialized = false;
        Draw::drawSelectedArea(output, selectedArea);
        return;
    }
    else
    {
        //If tracker was never set
        if (!tracker)
            return;
        
        if (!selectedArea.isSelected())
            return;
        
        if (!trackerInitialized)
        {
            tracker->initialize(frame, selectedArea.getBoundingBox());
            trackerInitialized = true;
        }
        else
        {
            tracker->processFrame(frame);
        }
        
        vector<Point2f> trackedArea;
        tracker->getTrackedArea(trackedArea);
        Draw::drawQuadrangle(output, trackedArea, Color::red);
        if (frameN == execution.size())
            execution.push_back(trackedArea);
        
    }
}



const Scalar Color::red  = Scalar(0,0,255);
const Scalar Color::blue = Scalar(255,0,0);
const Scalar Color::green= Scalar(0,255,0);
const Scalar Color::white= Scalar(255,255,255);
const Scalar Color::purple= Scalar(255,0,255);
const Scalar Color::yellow =  Scalar(0, 255, 255);
const Scalar Color::teal = Scalar(255, 255, 0);
const Scalar Color::orange = Scalar(22,134,241);



void Draw::drawSelectedArea(Mat &image,
                                  const RectSelectArea &area,
                                  const Scalar &color,
                                  int thickness)
{
    rectangle(image, area._loc[0], area._loc[1], color, thickness);
}


void Draw::drawQuadrangle(Mat &frameOut,
                                const vector<Point2f> &corners,
                                const Scalar &color,
                                const Point2f &shift,
                                const int thickness)
{
    drawQuadrangle(frameOut, corners[0],corners[1],corners[2],corners[3],color, shift, thickness);
}



void Draw::drawQuadrangle(Mat &frameOut,
                                const Point2f &one,
                                const Point2f &two,
                                const Point2f &three,
                                const Point2f &four,
                                const Scalar &color,
                                const Point2f &shift,
                                const int thickness)
{
    line( frameOut, one + shift, two + shift, color, thickness );
    line( frameOut, two + shift, three + shift, color, thickness );
    line( frameOut, three + shift, four + shift, color, thickness );
    line( frameOut, four + shift, one + shift, color, thickness );
}


