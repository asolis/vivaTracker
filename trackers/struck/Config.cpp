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

#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Config::Config(const std::string& path)
{
	SetDefaults();
	
	ifstream f(path.c_str());
	if (!f)
	{
		cout << "error: could not load config file: " << path << endl;
		return;
	}
	
	string line, name, tmp;
	while (getline(f, line))
	{
		istringstream iss(line);
		iss >> name >> tmp;
		
		// skip invalid lines and comments
		if (iss.fail() || tmp != "=" || name[0] == '#') continue;
		
		if      (name == "seed") iss >> seed;
		else if (name == "frameWidth") iss >> frameWidth;
		else if (name == "frameHeight") iss >> frameHeight;
		else if (name == "seed") iss >> seed;
		else if (name == "searchRadius") iss >> searchRadius;
		else if (name == "svmC") iss >> svmC;
		else if (name == "svmBudgetSize") iss >> svmBudgetSize;
		else if (name == "feature")
		{
			string featureName, kernelName;
			double param;
			iss >> featureName >> kernelName >> param;
			
			FeatureKernelPair fkp;
			
			if      (featureName == FeatureName(kFeatureTypeHaar)) fkp.feature = kFeatureTypeHaar;
			else if (featureName == FeatureName(kFeatureTypeRaw)) fkp.feature = kFeatureTypeRaw;
			else if (featureName == FeatureName(kFeatureTypeHistogram)) fkp.feature = kFeatureTypeHistogram;
			else
			{
				cout << "error: unrecognised feature: " << featureName << endl;
				continue;
			}
			
			if      (kernelName == KernelName(kKernelTypeLinear)) fkp.kernel = kKernelTypeLinear;
			else if (kernelName == KernelName(kKernelTypeIntersection)) fkp.kernel = kKernelTypeIntersection;
			else if (kernelName == KernelName(kKernelTypeChi2)) fkp.kernel = kKernelTypeChi2;
			else if (kernelName == KernelName(kKernelTypeGaussian))
			{
				if (iss.fail())
				{
					cout << "error: gaussian kernel requires a parameter (sigma)" << endl;
					continue;
				}
				fkp.kernel = kKernelTypeGaussian;
				fkp.params.push_back(param);
			}
			else
			{
				cout << "error: unrecognised kernel: " << kernelName << endl;
				continue;
			}
			
			features.push_back(fkp);
		}
	}
}

void Config::SetDefaults()
{
	frameWidth = 320;
	frameHeight = 240;
	
	seed = 0;
	searchRadius = 30;
	svmC = 100.0;
	svmBudgetSize = 100;
	
	// init the default feature kernel pair as specified in the original STRUCK config text file
	// note: multiple features can be combined
	features.clear();

	FeatureKernelPair fkp;
	fkp.feature = kFeatureTypeHaar;
	fkp.kernel = kKernelTypeGaussian;
	fkp.params.push_back(2.0);

	features.push_back(fkp);
}

void Config::SetParameters(const int &s, const int &radius, const double &sC, const int &budget, const std::string &f, const std::string &k)
{
	seed = s;
	searchRadius = radius;
	svmC = sC;
	svmBudgetSize = budget;

	features.clear();

	FeatureKernelPair fkp;
	if (f.compare("raw") == 0) fkp.feature = kFeatureTypeRaw;
	else if (f.compare("haar") == 0) fkp.feature = kFeatureTypeHaar;
	else if (f.compare("histogram") == 0) fkp.feature = kFeatureTypeHistogram;
	else
	{
		cout << "[STRUCKtracker] Feature type not specified, defaulting to haar features." << endl;
		fkp.feature = kFeatureTypeHaar;
	}

	if (k.compare("linear") == 0) fkp.kernel = kKernelTypeLinear;
	else if (k.compare("gaussian") == 0) fkp.kernel = kKernelTypeGaussian;
	else if (k.compare("intersection") == 0) fkp.kernel = kKernelTypeIntersection;
	else if (k.compare("chi2") == 0) fkp.kernel = kKernelTypeChi2;
	else
	{
		cout << "[STRUCKtracker] Kernel type not specified, defaulting to gaussian kernel." << endl;
		fkp.kernel = kKernelTypeGaussian;
	}

	fkp.params.push_back(2.0);
	features.push_back(fkp);
}


std::string Config::FeatureName(FeatureType f)
{
	switch (f)
	{
	case kFeatureTypeRaw:
		return "raw";
	case kFeatureTypeHaar:
		return "haar";
	case kFeatureTypeHistogram:
		return "histogram";
	default:
		return "";
	}
}

std::string Config::KernelName(KernelType k)
{
	switch (k)
	{
	case kKernelTypeLinear:
		return "linear";
	case kKernelTypeGaussian:
		return "gaussian";
	case kKernelTypeIntersection:
		return "intersection";
	case kKernelTypeChi2:
		return "chi2";
	default:
		return "";
	}
}

ostream& operator<< (ostream& out, const Config& conf)
{
	out << "config:" << endl;
	out << "  frameWidth         = " << conf.frameWidth << endl;
	out << "  frameHeight        = " << conf.frameHeight << endl;
	out << "  seed               = " << conf.seed << endl;
	out << "  searchRadius       = " << conf.searchRadius << endl;
	out << "  svmC               = " << conf.svmC << endl;
	out << "  svmBudgetSize      = " << conf.svmBudgetSize << endl;
	
	for (int i = 0; i < (int)conf.features.size(); ++i)
	{
		out << "  feature " << i << endl;
		out << "    feature: " << Config::FeatureName(conf.features[i].feature) << endl;
		out << "    kernel:  " << Config::KernelName(conf.features[i].kernel) <<endl;
		if (conf.features[i].params.size() > 0)
		{
			out << "    params: ";
			for (int j = 0; j < (int)conf.features[i].params.size(); ++j)
			{
				out << " " << conf.features[i].params[j];
			}
			out << endl;
		}
	}
	
	return out;
}