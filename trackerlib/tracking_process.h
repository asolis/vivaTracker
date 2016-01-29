/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __trackers__tracking_process__
#define __trackers__tracking_process__

#include "viva.h"
#include "tracker.h"

#include <fstream>


using namespace viva;
using namespace std;
using namespace cv;


class RectSelectArea
{
    int _state;
public:
    vector<Point2f> _loc;
    RectSelectArea();
    void mouseMove(int x, int y);
    void setClick(int x , int y);
    bool isSelected();
    Rect getBoundingBox();
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
    
    vector<vector<Point2f> > groundTruth;
    
public:
  
    TrackingProcess(const Ptr<Tracker> &trk, const vector<vector<Point2f> > &gt):
        tracker(trk), selectedArea(), trackerInitialized(false), groundTruth(gt)
    {}
    
    
    void setTracker(const Ptr<Tracker> &trk);
    //@Override
    void leftButtonDown(int x, int y, int flags);
    //@Override
    void mouseMove(int x, int y, int flags);
    //@Override
    void operator()(const size_t frameN, const Mat &frame, Mat &output);
    
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
