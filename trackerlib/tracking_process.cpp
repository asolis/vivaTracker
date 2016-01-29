/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/
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
    return boundingRect(_loc);
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


void Draw::drawPoints(Mat &image,
                            const vector<Point2f> &pts,
                            const Scalar &color,
                            int thickness)
{
    for (size_t i = 0; i < pts.size() ; i++)
    {
        line(image, pts[i], pts[i], color, thickness);
    }
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


