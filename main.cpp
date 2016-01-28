/*******************************************************
 * Copyright (C) 2016  Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "viva.h"
#include "factories.h"
#include <sstream>

using namespace viva;




void printHelp()
{
    cout << "vivaTracker [-t type] [-f feat] [-s] [-h] [-i] sequenceName" << endl;
    cout << endl;
    cout << "            type: [ g | l | p ]" << endl;
    cout << "                    g: gaussian   correlation (default)" << endl;
    cout << "                    l: polynomial correlation " << endl;
    cout << "                    p: linear     correlation " << endl;
    cout << endl;
    cout << "            feat: [ fhog | gray | rgb | hsv | hls ]" << endl;
    cout << endl;
    cout << "                    fhog: fast hog transform (default) " << endl;
    cout << "                    gray: gray scale feature " << endl;
    cout << "                    rgb:  rgb color feature " << endl;
    cout << "                    hsv:  hsv color feature " << endl;
    cout << "                    hls:  hls color feature " << endl;
    cout << endl;
    cout << "            -s: turn off scale (default on)" << endl;
    cout << "            -h: print this help (default off)" << endl;
}


string parseSequence(const string &sequence)
{
    string folder    = "/Users/andres/trackers/input/Benchmark/";
    string seq  = "Car4";
    
    auto buildPath = [&folder](const string &sequence)
    {
        ostringstream ss;
        ss << folder << sequence << "/img/";
        return ss.str();
    };
    
    if (viva::Files::exists(sequence))
        return sequence;
    
    string path = buildPath(sequence);
    if (viva::Files::exists(path))
        return path;
        
    
    return buildPath(seq);
}

int main(int argc, const char * argv[])
{
    const String keys =
        "{help h usage ?    |       | print this message}"
        "{@sequence         |       | video file, folder with images, image sequence, camera index}"
        "{t tracker         |skcf   | tracking algorithm to use}"
    ;
    
    CommandLineParser parser(argc, argv, keys);

   
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    

    string inputFolderWithImgs = parseSequence(parser.get<string>(0));
    
    //"/Users/andres/trackers/input/Benchmark/Car4/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/Couple/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/Suv/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/CarScale/img/";
    
    
    Processor processor;
    
    Ptr<Tracker> tracker = TrackerFactory::createTracker(TRACKING_METHOD::KCF_G_FHOG_S);
   /* Ptr<Tracker> tracker = TrackerFactory::createTracker(parseType(argc, argv),
                                                         parseFeat(argc, argv),
                                                         parseScale(argc, argv));*/
    
    Ptr<ProcessFrame> process = new TrackingProcess(tracker);
    Ptr<Input> input = new ImageListInput(inputFolderWithImgs, Size(-1,-1), -1, 0);
   
    processor.setInput(input);
    processor.setProcess(process);
    processor.listenToKeyboardEvents();
    processor.listenToMouseEvents();
    processor.run();

    
    return 0;
}