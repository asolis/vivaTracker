/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/
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
	void virtual configLoaded()
    {
        //readConfig();
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
        string prefix  = "", suffix = "";
        KType type = kcf.getType();
        KFeat feat = kcf.getFeature();
        
        switch (type)
        {
            case KType::GAUSSIAN:
            {
                prefix = "KCF(G)";
                break;
            }
            case KType::POLYNOMIAL:
            {
                prefix = "KCF(P)";
                break;
            }
            case KType::LINEAR:
            {
                prefix = "DCF";
                break;
            }
        }
        switch (feat)
        {
            case KFeat::GRAY:
            {
                suffix = "_G";
                break;
            }
            case KFeat::RGB:
            {
                suffix = "_RGB";
                break;
            }
            case KFeat::FHOG:
            {
                suffix = "_FHOG";
                break;
            }
            case KFeat::HSV:
            {
                suffix = "_HSV";
                break;
            }
            case KFeat::HLS:
            {
                suffix = "_HLS";
                break;
            }
                
        }
        return prefix + suffix + ((kcf.hasScale())?"_S":"");
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
