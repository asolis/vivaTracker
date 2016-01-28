/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef TRACKER_H
#define TRACKER_H

#include <string>
#include "utils.h"
#include "opencv2/opencv.hpp"


using namespace std;
using namespace cv;

/*
 * Tracker interface
 */
class Tracker
{

protected:
    string _postfix;
    string filename;
    
    
public:
    
  /*
   * This represent the current tracked area in clockwise order. e.g. : 
   * topLeft - > topRight -> bottomRight -> bottomLeft.
   * @param vector<Point2f> &pts. The tracked area will be filled  in this vector
   */
  void virtual getTrackedArea(vector<Point2f> &pts) = 0;
    
  /*
  * If used, this will return the tracked points at each time.
  * @param vector<Point2f> &pts. The tracked points will be filled in this vector.
  *                              If the algorithm doesn't track pts, will fill nothing.
  */
    void virtual getTrackedPoints(vector<Point2f> &pts) {};
    
    /*
     * If used, this will return the detected points at each time.
     * @param vector<Point2f> &pts. The detected points will be filled in this vector.
     *                              If the algorithm doesn't detect pts, will fill nothing.
     */
    void virtual getDetectedPoints(vector<Point2f> &pts) {};
    
    
  /*
   * Initialize the tracker using the first image with the selected area
   * using two points. The two points create a rectangular selection inside the image.
   * @param Mat &image,       The first reference frame where the area is specified.
   * @param Rect &rect  Rectangular region. 
   */
  void virtual initialize(const cv::Mat &image,
                          const cv::Rect &rect) = 0;
  /*
   * This should be called every time after the tracker is initialized.
   * The image is the current frame being processed.
   * @param Mat &image. processing frame.
   */
  void virtual processFrame(const cv::Mat &image) = 0;
    
  /*
   * Each tracker has an string description of its name
   * or condition.
   * e.g.  "OpenTLD" , "CMT", etc.
   */
   string virtual getDescription() { return "default";};
    
  /*
   * Just in case dynamic allocated memory needs to be destroyed
   * Abstract class should have a destructor....
   */
  virtual ~Tracker(){}
};





#endif // TRACKER_H
