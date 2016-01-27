/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __viva__listener__
#define __viva__listener__

#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

namespace viva
{
    class KeyboardListener
    {
    public:
        /*
         * It will be called only when the user stroke a keyboard key
         * @param key the value of the pressed key
         */
        virtual void keyboardInput(int key) {};
    };
    
    class MouseListener
    {
    public:
        /*
         * Will be triggered at any mouse event, the type of the event
         * is especified in the first integer @param event. 
         * event could be one of the following OpenCV values
         * EVENT_LBUTTONDOWN, EVENT_RBUTTONDOWN, EVENT_MBUTTONDOWN, EVENT_MOUSEMOVE. 
         * @param x the x coordinates of the mouse cursor
         * @param y the y coordinates of the mouse cursor
         * @param flgas the flags passed from OpenCV
         */
        virtual void mouseInput(int event, int x, int y, int flags){};
        
        /*
         * It will be called only when a mouse left click is triggered
         */
        virtual void leftButtonDown(int x, int y, int flags){};
        /*
         * It will be called only when a right left click is triggered
         */
        virtual void rightButtonDown(int x, int y, int flags){};
        /*
         * It will be called only when a mouse middle click is triggered
         */
        virtual void middleButtonDown(int x, int y, int flags){};
        /*
         * It will be called only when a mouse move is triggered.
         */
        virtual void mouseMove(int x, int y, int flags){};
    };
}
#endif /* defined(__h7__listener__) */
