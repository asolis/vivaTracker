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

#ifndef __viva__channel__
#define __viva__channel__

#include "opencv2/opencv.hpp"
#include <deque>
#include <condition_variable>

using namespace std;
using namespace cv;

namespace viva
{
  
    /**
     * BufferedChannel template class
     * The class is used to comunicate data between two threads 
     * using a producer-consurmer philosophy and a FIFO order.
     */
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
    
    /**
     *  A Buffered Channel of OpenCV Mat datatype. 
     */
    typedef BufferedChannel<Mat> BufferedImageChannel;
    
    
    
}

#endif
