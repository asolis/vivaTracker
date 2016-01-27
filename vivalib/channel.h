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
  
    
    template <class Data>
    class BufferedChannel
    {
    private:
        size_t _capacity;
        bool _terminate;
        float _fps;
        std::deque<Data> _images;
        std::mutex _access_queue;
        std::mutex _access_termination;
        std::mutex _access_fps;
        
        
        std::condition_variable _consume;
        std::condition_variable _produce;
        
    public:
        void close();
        
        bool isOpen();
        
        void addData(Data &data);
        
        bool empty();
        
        bool getData(Data &data);
        
        float getFrequency();
        void setFrequency(float frequency);
        void setCapacity(size_t capacity);
       
        BufferedChannel(size_t capacity = 10):
        _capacity(capacity), _terminate(false), _fps(0)
        {
            
        }
        
    };
    
    template<class Data>
    void BufferedChannel<Data>::setCapacity(size_t capacity)
    {
        std::lock_guard<std::mutex> guard(_access_queue);
        _capacity = capacity;
    }
    
    
    template<class Data>
    void BufferedChannel<Data>::close()
    {
        std::lock_guard<std::mutex> guard(_access_termination);
        _terminate = true;
        _consume.notify_all();
        _produce.notify_all();
    }
    template<class Data>
    bool BufferedChannel<Data>::isOpen()
    {
        std::lock_guard<std::mutex> guard(_access_termination);
        return !_terminate;
    }
    template<class Data>
    void BufferedChannel<Data>::addData(Data &data)
    {
    
        std::unique_lock<std::mutex> guard(_access_queue);
        _produce.wait(guard,[&] {
            return !isOpen() || (_images.size() < _capacity);
        });
        if (!isOpen())
        {
            guard.unlock();
            return;
        }
        _images.push_back(data);
        //cout<< "Added Image, size: " << _images.size() << endl;
        guard.unlock();
        _consume.notify_one();
    }
    template<class Data>
    bool BufferedChannel<Data>::empty()
    {
        std::lock_guard<std::mutex> guard(_access_queue);
        return _images.empty();
    }
    template<class Data>
    bool BufferedChannel<Data>::getData(Data &data)
    {
        std::unique_lock<std::mutex> guard(_access_queue);
        _consume.wait(guard, [&] {
            return (!isOpen() && _images.empty()) || !_images.empty();
        });
        if (_images.empty() && !isOpen())
        {
            guard.unlock();
            return false;
        }
    
        data = _images.front();
        _images.pop_front();
        guard.unlock();
        _produce.notify_one();
        return true;
    
    }
    
    template<class Data>
    float BufferedChannel<Data>::getFrequency()
    {
        std::lock_guard<std::mutex> guard(_access_fps);
        return _fps;
    }
    template<class Data>
    void BufferedChannel<Data>::setFrequency(float frequency)
    {
        std::lock_guard<std::mutex> guard(_access_fps);
        _fps = frequency;
    }
    
    
    typedef BufferedChannel<Mat> BufferedImageChannel;
    
    
    
}

#endif