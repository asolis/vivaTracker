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


#ifndef __VivaTracker__factory__
#define __VivaTracker__factory__

#include "precomp.h"
#include "tracking_process.h"


#ifdef WITH_SKCF
    #include "skcfdcf.h"
#endif

#ifdef WITH_KCF
    #include "kcftracker.hpp"
#endif 

#ifdef WITH_KCF2
    #include "kcf.h"
#endif

#ifdef WITH_NCC
    #include "ncc.h"
#endif

#ifdef WITH_OPENTLD
    #include "opentld.h"
#endif 

#ifdef WITH_STRUCK
    #include "STRUCKtracker.h"
#endif 


/**
 * TrackerFactory class 
 * A set of static class to generate a video sequence input , output and tracker implementation.
 * It uses the vivalib interfaces Input, Ouput and the Tracker interface
 */
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

    /**
     * Giving a string it determines what kind of sequence could be loaded and 
     * returns an object follwing the vivalib::Input interface
     */
    static Ptr<Input> createInput(const string &sequence);
    /**
     * Giving a string it determines what kind of output method to create and 
     * returns an object following the vivalib::Output interface
     */
    static Ptr<Output> createOutput(const string &filename);
    /**
     * Creates a tracker using the **method** identifier and passing the command line arguments 
     * for futher options configuration of the tracker.
     * This method should be used to create and return the current available trackers in the project.
     * see project's wiki for more details how to create your own tracker
     */
    static Ptr<Tracker> createTracker(const string &method, const int argc, const char * argv[]);
    /**
     * loads the groundtruth file from a filename if available into a 2D list of points 
     */
    static void loadGroundTruth(const string &sequence, vector<vector<Point2f> > &groundTruth);
};

/**
 *  GroundTruth class
 *  Parses a file content by generating the list of areas for each frame in the video sequence
 */
class GroundTruth
{
    
public:

    /**
     * splits a string using delimitator character and create a vector with all the splitted values
     */
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
    
    /**
     *  parses and loads a ground-truth file into a 2D list of points.
     * @param file: filename of the file containing the ground-truth annotations
     * @param gt: outputs the ground-truth in a 2D list of points.
     */
    static void  parse(const string &file, vector<vector<Point2f> > &gt);
};

#endif /* defined(__VivaTracker__tracker_factory__) */
