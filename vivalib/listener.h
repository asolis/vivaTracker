/**************************************************************************************************
 **************************************************************************************************
 
     BSD 3-Clause License (https://www.tldrlegal.com/l/bsd3)
     
     Copyright (c) 2015 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.
     
     
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

#ifndef __viva__listener__
#define __viva__listener__

#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

namespace viva
{
    /**
     * KeyboardListener interface
     */
    class KeyboardListener
    {
    public:
        /**
         * It will be called only when the user stroke a keyboard key
         * @param key the value of the pressed key
         */
        virtual void keyboardInput(int key) {};
    };

    /**
     * MouseListener Interface
     */
    class MouseListener
    {
    public:
        /**
         * Will be triggered at any mouse event, the type of the event
         * is especified in the first integer @param event. 
         * event could be one of the following OpenCV values
         * EVENT_LBUTTONDOWN, EVENT_RBUTTONDOWN, EVENT_MBUTTONDOWN, EVENT_MOUSEMOVE. 
         * @param x the x coordinates of the mouse cursor
         * @param y the y coordinates of the mouse cursor
         * @param flgas the flags passed from OpenCV
         */
        virtual void mouseInput(int event, int x, int y, int flags){};
        
        /**
         * It will be called only when a mouse left click is triggered
         */
        virtual void leftButtonDown(int x, int y, int flags){};
        /**
         * It will be called only when a right left click is triggered
         */
        virtual void rightButtonDown(int x, int y, int flags){};
        /**
         * It will be called only when a mouse middle click is triggered
         */
        virtual void middleButtonDown(int x, int y, int flags){};
        /**
         * It will be called only when a mouse move is triggered.
         */
        virtual void mouseMove(int x, int y, int flags){};
    };
}
#endif /* defined(__h7__listener__) */
