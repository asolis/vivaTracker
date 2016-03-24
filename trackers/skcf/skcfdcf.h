/**************************************************************************************************
 **************************************************************************************************
 
     GPL-3 License (https://www.tldrlegal.com/l/gpl-3.0)
     
     Copyright (c) 2015 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.

 sKCF is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
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
