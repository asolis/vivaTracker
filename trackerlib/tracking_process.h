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

/**
 *  RecSelectArea class
 *
 *  Defines a axis-aligned rectangular selection area
 *  by using the top-left and bottom-right corner selections
 *
 */
class RectSelectArea
{
    int _state; /**< different states of a selection area: 0, 1, 2*/
public:
    vector<Point2f> _loc;
    /**
     * Default constructor
     */
    RectSelectArea();
    /**
     * Updates location of the bottom right corner to the specified
     * images coordinates x and y
     * @param x: x-coordinate value in the image
     * @param y: y-coordinate value in the image
     */
    void mouseMove(int x, int y);
    /**
     *  Updates the corner locations corresponding to the 
     *  current state.
     * @param x: x-coordinate value in the image
     * @param y: y-coordinate value in the image
     */
    void setClick(int x , int y);
    /**
     *  Returns if the rectangular area has a complete selection
     *  _state has a value of 2
     */
    bool isSelected();
    /**
     * Return the Rectangular area defined by the two corners
     */
    Rect getBoundingBox();
};

/**
 *  Color struct. 
 *  Custom color codes used by the Draw class
 *  The color are specified in BGR format (i.e., blue, gree, and red)
 */
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


/**
 * TrackingProcess class
 * Inherites from vivalib ProcessFrame class to define a
 * functor class that is called each time an input frame is available form the
 * sequence and also handles mouse and keyboard events.
 */
class TrackingProcess: public ProcessFrame
{
    Ptr<Tracker> tracker; /**< Pointer to tracker interface, holds the tracking algorithm created by TrackerFactory*/
    RectSelectArea selectedArea; /**< Rectangular area selection in the video sequence */
    bool trackerInitialized;  /**< Identifies if the tracker has been initialized or not */
    vector<vector<Point2f> > groundTruth; /**< ground truth data if available to the sequence*/
    vector<vector<Point2f> > execution; /**< tracking area recorded for the sequence*/
public:

    /**
     * Constructor of a tracking process
     * @see Tracker
     * @see TrackerFactory
     * @param trk : pointer to a Tracker object. 
     * @param gt:  ground-truth defined as a 2D list points. 
     * The ground-truth area for frame number N can be found by gt[N].
     */
    TrackingProcess(const Ptr<Tracker> &trk, const vector<vector<Point2f> > &gt):
        tracker(trk), selectedArea(), trackerInitialized(false), groundTruth(gt), execution()
    {}

    /*
     *  Set pointer to tracking algorithm
     *  @param trk: traking algorithm
     */
    void setTracker(const Ptr<Tracker> &trk);
    /**
     * Override from ProcessFrame class in vivalib
     * Handles mouse left clicks. Used to defined new rectangular selection areas 
     * in the displayed windows and intilialize the tracker.
     * @param x: x-coordinate of the clicked pixel in the image
     * @param y: y-coordinate of the clicked pixel in the image
     * @param flags: OpenCV flags for mouse clicks
     */
    void leftButtonDown(int x, int y, int flags);
    /**
     * Override from ProcessFrame class in vivalib
     * Handles mouse movements over the displayed window.
     * @param x: x-coordinate of the current location of the mouse in the displayed window
     * @param y: y-coordinate of the current location of the mouse in the displayed window
     * @param flags: OpenCV flag
     */
    void mouseMove(int x, int y, int flags);
    /**
     * Functor operator called each time that an input frame is available
     * @param frameN: number of the frame
     * @param frame: the frame image
     * @param output: the processed tracking output image
     *
     */
    void operator()(const size_t frameN, const Mat &frame, Mat &output);

    /**
     * Returns the currently tracking area info. 
     * Annotated areas for each frame in the sequence.
     * It has the following format for each frame:
     * x1, y1, x2, y2, x3, y3, x4, y4
     */
    void getTrackingInfo(vector<vector<Point2f> > &pts)
    {
        pts = execution;
    }
};

/**
 * Util class for drawing basic figures over the video sequence
 *
 */
class Draw
{
public:

    /**
     * Draws the Rectangular selected area over the image using the specified color and thickness.
     * @param image: image canvas where to draw the selection area
     * @param area: the selection area to draw
     * @param color: color of selected area lines
     * @param thicknes: thickness of the selected area lines
     */
    static void drawSelectedArea(Mat &image,
                                 const RectSelectArea &area,
                                 const Scalar &color = Color::red,
                                 int thickness = 3);

    /**
     * Draws a four-sided polygon defined for a list of points 
     * @param frameOut: the image canvas where to draw
     * @param corners: list of corners locations of the four-sided polygon
     * @param color: color of selected area lines
     * @param shift: vector to add to each corner before drawing, by default vector(0,0)
     * @param thicknes: thickness of the selected area lines
     */
    static void drawQuadrangle(Mat &frameOut,
                               const vector<Point2f> &corners,
                               const Scalar &color,
                               const Point2f &shift = Point2f(0,0),
                               const int thickness = 2);

    /**
     * Draws a four-sided polygon defined by four points instead of a list
     * @see drawQuadrangle(Mat &frameOut, const vector<Point2f> &corners, ...)
     */
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
