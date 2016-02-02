/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __VivaTracker__factory__
#define __VivaTracker__factory__

#include <iostream>

#include "tracker.h"
#include "tracking_process.h"
#include "skcfdcf.h"
#include "kcftracker.hpp"
#include "ncc.h"
#include "opentld.h"
#include "STRUCKtracker.h"

class TrackerFactory
{
    
private:
    static string SEQ_BASE_FILE;
    static string GROUND_TRUTH_FILE;
    
    static bool isVideoFile(const string &sequence);
    static bool isWebFile(const string &sequence);
    static bool isStringSequence(const string &sequence);
    static bool isFolderSequence(const string &sequence);
    static string constructSequenceFolder(const string &file, const string &sequence);

    
public:
    
    static Ptr<Input> createInput(const string &sequence);
    static Ptr<Output> createOutput(const string &filename);
    static Ptr<Tracker> createTracker(const string &method, const int argc, const char * argv[]);
    static void loadGroundTruth(const string &sequence, vector<vector<Point2f> > &groundTruth);
};

class GroundTruth
{
    
public:
    
    template<typename T>
    static std::vector<T> &split(const std::string &s, char delim, std::vector<T> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            T tmp;
            std::stringstream(item) >> tmp;
            elems.push_back(tmp);
        }
        return elems;
    }
    
    
    static void  parse(const string file, vector<vector<Point2f> > &gt);
};

#endif /* defined(__VivaTracker__tracker_factory__) */
