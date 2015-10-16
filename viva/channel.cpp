/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "channel.h"

using namespace viva;

void BufferedImageChannel::close()
{
    std::lock_guard<std::mutex> guard(_access_termination);
    _terminate = true;
    _consume.notify_all();
    _produce.notify_all();
}

bool BufferedImageChannel::isOpen()
{
    std::lock_guard<std::mutex> guard(_access_termination);
    return !_terminate;
}

void BufferedImageChannel::addImage(Mat &image)
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
    _images.push_back(image);
    //cout<< "Added Image, size: " << _images.size() << endl;
    guard.unlock();
    _consume.notify_one();
}

bool BufferedImageChannel::empty()
{
    std::lock_guard<std::mutex> guard(_access_queue);
    return _images.empty();
}

bool BufferedImageChannel::getFrame(Mat &image)
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
    
    image = _images.front();
    _images.pop_front();
    guard.unlock();
    _produce.notify_one();
    return true;
    
}


float BufferedImageChannel::getFrequency()
{
    std::lock_guard<std::mutex> guard(_access_fps);
    return _fps;
}

void BufferedImageChannel::setFrequency(float frequency)
{
    std::lock_guard<std::mutex> guard(_access_fps);
    _fps = frequency;
}