 /*******************************************************
 * Copyright (C) 2013 Andrés Solís Montero <asolis@hotmail.ca>
 *   PhD Student.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 * Project can not be copied and/or distributed without the express
 * permission of Andrés Solís Montero
 * Created by Andres Solis Montero on 2014-06-18.
 *******************************************************/
#include "tracking_process.h"

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
void TrackingProcess::operator()(size_t frameN, Mat &frame, Mat &output)
{

    frame.copyTo(output);

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
        
        vector<Point2f> trackedPts;
        tracker->getTrackedPoints(trackedPts);
        Draw::drawPoints(output, trackedPts, Color::white);
        
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


