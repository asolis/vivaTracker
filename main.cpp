/*******************************************************
 *   Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "viva.h"
#include "factories.h"

using namespace viva;

int main(int argc, const char * argv[])
{
    
    string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/Car4/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/Couple/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/Suv/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/CarScale/img/";
    string outputVideoFile     = "video.avi";
    
    Processor processor(argc, argv);
    
    
    Ptr<Tracker> tracker = TrackerFactory::createTracker(TRACKING_METHOD::KCF_G_FHOG_S);
    Ptr<ProcessFrame> process = new TrackingProcess(tracker);
    
    Ptr<Input> input = new ImageListInput(inputFolderWithImgs);
   
    processor.setInput(input);
    processor.setProcess(process);
    processor.listenToKeyboardEvents();
    processor.showTimeInfo();
    processor.listenToMouseEvents();
    processor.run();

    
    return 0;
}