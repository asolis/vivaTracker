/* 
 * Struck: Structured Output Tracking with Kernels
 * 
 * Code to accompany the paper:
 *   Struck: Structured Output Tracking with Kernels
 *   Sam Hare, Amir Saffari, Philip H. S. Torr
 *   International Conference on Computer Vision (ICCV), 2011
 * 
 * Copyright (C) 2011 Sam Hare, Oxford Brookes University, Oxford, UK
 * 
 * This file is part of Struck.
 * 
 * Struck is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Struck is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Struck.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef __trackers__STRUCKTracker__
#define __trackers__STRUCKTracker__

#include "Rect.h"
#include "Config.h"

#include <vector>
#include <Core>
#include <opencv/cv.h>

#include "tracker.h"
#include "viva.h"

//class Config;
class Features;
class Kernel;
class LaRank;
class ImageRep;

using namespace cv;
using namespace std;
using namespace viva;

class STRUCKtracker : public Tracker
{
public:
	STRUCKtracker();
	~STRUCKtracker();

	void virtual configLoaded()
	{
        int seed         = 0;
        int searchRadius = 30;
        double svmC      = 100.0;
        int svmBudgetSize= 100.0;

		string featureType = "haar";
		string kernalType = "gaussian";

		m_config.SetParameters(seed, searchRadius, svmC, svmBudgetSize, featureType, kernalType);

		Reset();
	}
	
    //@Override
    void initialize(const cv::Mat &im_gray, const cv::Rect &rect)
	{
		Reset();
		FloatRect bb(rect.x, rect.y, rect.width, rect.height);
		if (bb.Area() > 0)
		{
			m_config.frameWidth = rect.width;
			m_config.frameHeight = rect.height;
			Init(im_gray, bb);
		}
	}

    //@Override
    string virtual getDescription()
    {
        return "Sam Hare, Amir Saffari, Philip H. S. Torr. Struck: Structured Output Tracking with Kernels. 2011";
    }
    
    //@Override
    void virtual getTrackedArea(vector<Point2f> &pts)
    {
		pts.push_back(Point2f(m_bb.XMin(), m_bb.YMin())); // topLeft
		pts.push_back(Point2f(m_bb.XMax(), m_bb.YMin())); // topRight
		pts.push_back(Point2f(m_bb.XMax(), m_bb.YMax())); // bottomRight
		pts.push_back(Point2f(m_bb.XMin(), m_bb.YMax())); // bottomLeft
    }

    
    //@Override
    void processFrame(const Mat &im_gray)
	{
		if (m_initialised)
		{
			Track(im_gray);
		}
	}

	// original STRUCK functions
	void Init(const cv::Mat& frame, FloatRect bb);
	void Reset();
	void Track(const cv::Mat& frame);
	
	inline const FloatRect& GetBB() const { return m_bb; }
	inline bool IsInitialised() const { return m_initialised; }
	
private:
	Config m_config;
	bool m_initialised;
	std::vector<Features*> m_features;
	std::vector<Kernel*> m_kernels;
	LaRank* m_pLearner;
	FloatRect m_bb;
	bool m_needsIntegralImage;
	bool m_needsIntegralHist;
	
	void UpdateLearner(const ImageRep& image);
};

#endif
