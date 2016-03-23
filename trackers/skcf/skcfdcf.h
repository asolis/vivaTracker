/**************************************************************************************************
 **************************************************************************************************
 
     BSD 3-Clause License (https://www.tldrlegal.com/l/bsd3)
     
     Copyright (c) 2015 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.
     
     
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
#ifndef __trackers__SKCFDCF__
#define __trackers__SKCFDCF__

#include "tracker.h"
#include "utils.h"
#include "ktrackers.h"
#include <sstream>


class SKCFDCF :	public Tracker
{
private:
    
    KTrackers kcf;
    

public:
    SKCFDCF(KType type = KType::GAUSSIAN,
            KFeat feat = KFeat::FHOG ,
            bool scale = false): kcf(type, feat, scale){}
    
	~SKCFDCF(void)
	{
	}

	void setArea(const RotatedRect &rect)
    {
        kcf.setArea(rect);
    }
    
    //@Override
    void initialize(const cv::Mat &image,
                    const cv::Rect &rect)
    {
        float width =rect.width;
        float height=rect.height;
        Point2f center = Point2f(rect.tl().x + width/2.0,
                                 rect.tl().y + height/2.0);
        //update target size and location
        Size2f targetSize(width, height);
        kcf.setArea(RotatedRect(center,targetSize,0));
        kcf.processFrame(image);
    }
    

  
    //@Override
    string virtual getDescription()
    {
        return "Andrés Solís Montero. sKCF: Scalable Kernel Correlation Filter with Sparse Feature Integration. 2015";
    }
    
    
   
    
    //@Override
    void getTrackedPoints(vector<Point2f> &pts)
    {
       // kcf.getTrackedPoints(pts);
    }
    
    //@Override
    void virtual getTrackedArea(vector<Point2f> &pts)
    {
        kcf.getTrackedArea(pts);
        
    }
    
    //@Override
    void processFrame(const cv::Mat &frame)
    {
        kcf.processFrame(frame);
    }
};
#endif
