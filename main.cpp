/*******************************************************
 *   Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "core.h"

using namespace core;


class EdgeProcessor: public ProcessFrame
{
public:
    void operator()(size_t frameN, Mat &frame, Mat &output)
    {
       Canny(frame, output, 5,10);
    }
    
    virtual void mouseInput(int event, int x, int y, int flags)
    {
        cout << "Mouse Input: " << event << " x:" << x << " y:" << y << " flags: " << flags << endl;
    }
    
    virtual void leftButtonDown(int x, int y, int flags)
    {
        cout << "Mouse Left Click: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
    }
    
    virtual void rightButtonDown(int x, int y, int flags)
    {
        cout << "Mouse Right Click: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
    }
    
    virtual void middleButtonDown(int x, int y, int flags)
    {
        cout << "Mouse Middle Click: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
    }
    
    virtual void mouseMove(int x, int y, int flags)
    {
        cout << "Mouse Move: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
    }
    
    virtual void keyboardInput(int key)
    {
        cout << "Keyboard Input: " << key << endl;
    }
};

void filter(size_t frameN, Mat &frame, Mat &output)
{
    Canny(frame, output, 5,10);
}

class Filter
{
public:
    void operator()(size_t frameN, Mat &frame, Mat &output)
    {
        Canny(frame, output, 5,10);
    }
};

auto x = [](size_t frameN, Mat &frame, Mat &output)
{
        Canny(frame, output, 5,10);
};

int main(int argc, const char * argv[])
{
    Processor processor;
    
    Ptr<Output> output = new VideoOutput("a.mov");
    processor.setOutput(output);
    
    processor.run();
    
    return 0;
}