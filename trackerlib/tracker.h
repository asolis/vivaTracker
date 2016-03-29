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

#ifndef TRACKER_H
#define TRACKER_H

#include <string>
#include "utils.h"
#include "opencv2/opencv.hpp"


using namespace std;
using namespace cv;

/**
 * Tracker interface
 */
class Tracker
{
public:
    
  /**
   * This represent the current tracked area in clockwise order. e.g. : 
   * topLeft - > topRight -> bottomRight -> bottomLeft.
   * @param vector<Point2f> &pts. The tracked area will be filled  in this vector
   */
  void virtual getTrackedArea(vector<Point2f> &pts) = 0;
    
  /**
   * Initialize the tracker using the first image with the selected area
   * using two points. The two points create a rectangular selection inside the image.
   * @param Mat &image,       The first reference frame where the area is specified.
   * @param Rect &rect  Rectangular region. 
   */
  void virtual initialize(const cv::Mat &image,
                          const cv::Rect &rect) = 0;
  /**
   * This should be called every time after the tracker is initialized.
   * The image is the current frame being processed.
   * @param Mat &image. processing frame.
   */
  void virtual processFrame(const cv::Mat &image) = 0;
    
  /**
   * Each tracker has an string description of its name
   * or condition.
   * e.g.  "OpenTLD" , "CMT", etc.
   */
   string virtual getDescription() { return "default";};
    
  /**
   * Just in case dynamic allocated memory needs to be destroyed
   * Abstract class should have a destructor....
   */
  virtual ~Tracker(){}
};





#endif // TRACKER_H
