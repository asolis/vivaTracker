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
