/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "core.h"

using namespace core;

#include <iostream>


int main(int argc, const char * argv[])
{
    
    
    Processor processor(argc, argv);
    
    //Ptr<Input> input = new CameraInput(0, Size(640,480), true);
    Ptr<Input> input = new VideoInput("http://root:viva2014@173.220.183.156:8890/mjpg/video.mjpg");
    processor.setInput(input);
    

    Ptr<ProcessFrame> process = new IOProcess();
    processor.setProcess(process);
    
    
    Ptr<Output> output = new VideoOutput("output.avi");
    processor.setOutput(output);
    
    //You can add keyboard events to your Process (See KeyboardListener in listener.h)
    processor.addKeyboardListener();
    
    //Adding mouse events to your Process (See MouseListener in listener.h)
    processor.addMouseListener();
    
    
    processor.run();
    
    
    return 0;
}