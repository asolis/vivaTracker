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

#ifndef __trackers__tracking_process__
#define __trackers__tracking_process__

#include "viva.h"
#include "tracker.h"


using namespace viva;
using namespace std;
using namespace cv;


class RectSelectArea
{
    int _state;
    
public:
    vector<Point2f> _loc;
    
    RectSelectArea():
        _state(0), _loc(2, Point2f(-1,-1))
    {
        _loc[0] = Point2f(-1,-1);
        _loc[1] = Point2f(-1,-1);
    }
    
    void mouseMove(int x, int y)
    {
        if (_state == 1)
        _loc[1] = Point2f(x,y);
    }
    
    void setClick(int x , int y)
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
    bool isSelected()
    {
        return _state == 2;
    }

    Rect getBoundingBox()
    {
        return boundingRect(_loc);
    }
};

struct Color
{
    const static Scalar red;
    const static Scalar blue;
    const static Scalar green;
    const static Scalar white;
    const static Scalar purple;
    const static Scalar yellow;
    const static Scalar teal;
    const static Scalar orange;
};

class TrackingProcess: public ProcessFrame
{
    Ptr<Tracker> tracker;
    RectSelectArea selectedArea;
    bool trackerInitialized;
    
public:
    TrackingProcess(): TrackingProcess(NULL)
    {}
    
    TrackingProcess(const Ptr<Tracker> &trk):
        tracker(trk), selectedArea(), trackerInitialized(false)
    {}
    
    void setTracker(const Ptr<Tracker> &trk);
    //@Override
    void leftButtonDown(int x, int y, int flags);
    //@Override
    void mouseMove(int x, int y, int flags);
    //@Override
    void operator()(size_t frameN, Mat &frame, Mat &output);
    
};




class Draw
{

    
public:

    
    static void drawPoints(Mat &image,
                           const vector<Point2f> &pts,
                           const Scalar &color,
                           int thickness = 2);
    
    static void drawSelectedArea(Mat &image,
                                 const RectSelectArea &area,
                                 const Scalar &color = Color::red,
                                 int thickness = 3);
    

    
    static void drawQuadrangle(Mat &frameOut,
                               const vector<Point2f> &corners,
                               const Scalar &color,
                               const Point2f &shift = Point2f(0,0),
                               const int thickness = 2);
    
  
    
    static void drawQuadrangle(Mat &frameOut,
                               const Point2f &one,
                               const Point2f &two,
                               const Point2f &three,
                               const Point2f &four,
                               const Scalar &color,
                               const Point2f &shift = Point2f(0,0),
                               const int thickness = 2);
    
    
};

#endif /* defined(__h7__tracking_process__) */
