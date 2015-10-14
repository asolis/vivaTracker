/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __core__process__
#define __core__process__

#ifdef _MSC_VER
#define NOMINMAX
#endif

#include <iostream>
#include "listener.h"
#include "utils.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

namespace core
{
    class ProcessFrame : public MouseListener, public KeyboardListener
    {
    public:
        virtual ~ProcessFrame(){}
        virtual void execute(long frameN, Mat &frame, Mat &output) = 0;

    };

    
    
    class NoneProcess: public ProcessFrame
    {
    public:
        void execute(long frameN, Mat &frame, Mat &output)
        {
            output = frame.clone();
        }
    };
    
    
    class IOProcess: public ProcessFrame
    {
    public:
        void execute(long frameN, Mat &frame, Mat &output)
        {
            output = frame.clone();
        }
       
        
        virtual void mouseInput(int event, int x, int y, int flags)
        {
                cout << "Mouse Input: " << event << " x:" << x << " y:" << y << " flags: " << flags << endl;
        };
        
        virtual void leftButtonDown(int x, int y, int flags)
        {
             cout << "Mouse Left Click: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
        };
        
        virtual void rightButtonDown(int x, int y, int flags)
        {
             cout << "Mouse Right Click: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
        };
        
        virtual void middleButtonDown(int x, int y, int flags)
        {
             cout << "Mouse Middle Click: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
        };
        
        virtual void mouseMove(int x, int y, int flags)
        {
             cout << "Mouse Move: " << " x:" << x << " y:" << y << " flags: " << flags << endl;
        };
        
        virtual void keyboardInput(int key)
        {
            cout << "Keyboard Input: " << key << endl;
        };
    };
}



#endif /* defined(__core__process__) */
