/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/


#ifndef __viva__channel__
#define __viva__channel__

#include "opencv2/opencv.hpp"
#include <deque>
#include <condition_variable>

using namespace std;
using namespace cv;

namespace viva
{
    class BufferedImageChannel
    {
    private:
        size_t _capacity;
        bool _terminate;
        float _fps;
        std::deque<Mat> _images;
        std::mutex _access_queue;
        std::mutex _access_termination;
        std::mutex _access_fps;
        
        
        std::condition_variable _consume;
        std::condition_variable _produce;
        
    public:
        void close();
        
        bool isOpen();
        void addImage(Mat &image);
        
        bool empty();
        
        bool getFrame(Mat &image);
        float getFrequency();
        void setFrequency(float frequency);
        
        BufferedImageChannel(size_t capacity = 10):
        _capacity(capacity), _terminate(false), _fps(0)
        {
            
        }
    };
}

#endif