/*******************************************************
 *   Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
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

KType parseType(int argc, const char * argv[])
{
    KType type = KType::GAUSSIAN;
    for (int i = 0; i < argc; i++)
    {
        
        if (string(argv[i]) == "-t")
        {
            if (i + 1 >= argc) break;
            
            string _t = argv[i+1];
            if (_t == "g")
                type = KType::GAUSSIAN;
            if (_t == "l")
                type = KType::LINEAR;
            if (_t == "p")
                type = KType::POLYNOMIAL;
            
            cout << "selected " << _t << endl; 
        }
    }
    return type;
}

KFeat parseFeat(int argc, const char * argv[])
{
    KFeat feat = KFeat::FHOG;
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-f")
        {
            if (i + 1 >= argc) break;
            string _t = argv[i+1];
            if (_t == "fhog")
                feat = KFeat::FHOG;
            if (_t == "gray")
                feat = KFeat::GRAY;
            if (_t == "rgb")
                feat = KFeat::RGB;
            if (_t == "hsv")
                feat = KFeat::HSV;
            if (_t == "hls")
                feat = KFeat::HLS;
            
            cout << "selected " << _t << endl;
            break;
        }
    }
    return feat;
}

bool parseScale(int argc, const char * argv[])
{
    bool scale = true;
    
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-s")
        {
            scale = false;
        }
    }
    
    return scale;
}

bool parseHelp(int argc, const char * argv[])
{

    
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-h")
        {
            printHelp();
            return true;
        }
    }
    return false;
}
bool parseInfo(int argc, const char * argv[])
{
    
    
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-i")
        {
            return true;
        }
    }
    return false;
}

string parseSequence(int argc, const char * argv[])
{
    
    string folder    = "/Users/andres/trackers/input/Benchmark/";
    string sequence  = "Car4";
    
    auto buildPath = [&folder](const string &sequence)
    {
        ostringstream ss;
        ss << folder << sequence << "/img/";
        return ss.str();
    };
    
    
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]).substr(0,1) == "-")
            continue;
        else
        {
            string path = buildPath(argv[i]);
            if (viva::Files::exists(path))
                return path;
        }
    }
    
    return buildPath(sequence);
    
}



int main(int argc, const char * argv[])
{
    

    if (parseHelp(argc, argv))
        return 0;

    string inputFolderWithImgs = parseSequence(argc, argv); //"/Users/andres/trackers/input/Benchmark/Car4/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/Couple/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/Suv/img/";
    //string inputFolderWithImgs = "/Users/andres/trackers/input/Benchmark/CarScale/img/";
    
    
    Processor processor(argc, argv);
    
    //Ptr<Tracker> tracker = TrackerFactory::createTracker(TRACKING_METHOD::KCF_G_FHOG_S);
    Ptr<Tracker> tracker = TrackerFactory::createTracker(parseType(argc, argv),
                                                         parseFeat(argc, argv),
                                                         parseScale(argc, argv));
    
    Ptr<ProcessFrame> process = new TrackingProcess(tracker);
    Ptr<Input> input = new ImageListInput(inputFolderWithImgs, Size(-1,-1), -1, 0);
   
    processor.setInput(input);
    processor.setProcess(process);
    processor.listenToKeyboardEvents();
    if (parseInfo(argc, argv))
        processor.showTimeInfo();
    processor.listenToMouseEvents();
    processor.run();

    
    return 0;
}