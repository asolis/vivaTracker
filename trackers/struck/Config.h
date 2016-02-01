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

#ifndef __STRUCK_CONFIG_H__
#define __STRUCK_CONFIG_H__

#include <vector>
#include <string>
#include <ostream>

#define VERBOSE (0)

class Config
{
public:
	Config() { SetDefaults(); }
	Config(const std::string& path);
	
	enum FeatureType
	{
		kFeatureTypeHaar,
		kFeatureTypeRaw,
		kFeatureTypeHistogram
	};

	enum KernelType
	{
		kKernelTypeLinear,
		kKernelTypeGaussian,
		kKernelTypeIntersection,
		kKernelTypeChi2
	};

	struct FeatureKernelPair
	{
		FeatureType feature;
		KernelType kernel;
		std::vector<double> params;
	};
	
	// In the original STRUCK source the frameWidth and frameHeight parameters were used to scale the image
	// used in tracking. The config text file supplied in the original soure specified a (w,h) of (320, 240) 
	int								frameWidth;
	int								frameHeight;
	
	int								seed;
	int								searchRadius;
	double							svmC;
	int								svmBudgetSize;
	std::vector<FeatureKernelPair>	features;
	
	friend std::ostream& operator<< (std::ostream& out, const Config& conf);

	// manual setting parameters
	// s = seed
	// radius = searchRadius
	// sC = svmC
	// budget = svmBudgetSize
	// f = featureType
	// k = kernelType
	void SetParameters(const int &s, const int &radius, const double &sC, const int &budget, const std::string &f, const std::string &k);
	
private:
	void SetDefaults();
	static std::string FeatureName(FeatureType f);
	static std::string KernelName(KernelType k);
};

#endif