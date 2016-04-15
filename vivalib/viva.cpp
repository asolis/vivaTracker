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

#include "viva.h"

using namespace viva;


void ProcessInput::operator()()
{
    if (!_input)
    {
        _channel->close();
        return;
    }
    while (_channel->isOpen())
    {
        Mat frame;
        auto start_time = chrono::high_resolution_clock::now();
        
        bool hasFrame = _input->getFrame(frame);

        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        _channel->setFrequency((float)(1000.0/double(duration)));
        
        if (!hasFrame || frame.empty())
        {
            _channel->close();
        }
        else
        {
            _channel->addData(frame);

        }
        
    }
}


void ProcessOutput::operator()()
{
    if (!_output)
    {
        _channel->close();
        return;
    }
    while (_channel->isOpen())
    {
        Mat frame;
        bool hasFrame = _channel->getData(frame);
        if (!hasFrame || frame.empty())
            _channel->close();
        else
        {
            auto start_time = chrono::high_resolution_clock::now();
            _output->writeFrame(frame);
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            _channel->setFrequency((float)(1000.0/double(duration)));
        }
        
    }
}



void Processor::mouseCallback(int event, int x, int y, int flags, void *ptr)
{
    MouseListener* listener = (MouseListener*)ptr;
    if (listener)
    {
        listener->mouseInput(event, x, y, flags);
        
        if (event == EVENT_LBUTTONDOWN)
        {
            listener->leftButtonDown(x, y, flags);
        }
        else if (event == EVENT_RBUTTONDOWN)
        {
            listener->rightButtonDown(x, y, flags);
        }
        else if (event == EVENT_MBUTTONDOWN)
        {
            listener->middleButtonDown(x, y, flags);
        }
        else if (event == EVENT_MOUSEMOVE)
        {
            listener->mouseMove(x, y, flags);
        }
    }
}


void Processor::run()
{
    
    int FLAGS = CV_GUI_NORMAL | CV_WINDOW_AUTOSIZE;
    
    if (_showInput)
        namedWindow(_inputWindowName,FLAGS);
    if (_showOutput)
        namedWindow(_outputWindowName, FLAGS);
    if (_mListener && _process)
        cv::setMouseCallback(_outputWindowName, Processor::mouseCallback, _process);
    
    if (!_input && (!_process || !_functor))
        return;
    
    Ptr<BufferedImageChannel> _input_channel = new BufferedImageChannel(_inputBufferSize);
    std::thread  _inputThread(ProcessInput(_input, _input_channel));
    thread_guard gi(_inputThread);
    
    Ptr<BufferedImageChannel> _output_channel = new BufferedImageChannel(_outputBufferSize);
    std::thread  _outputThread(ProcessOutput(_output, _output_channel));
    thread_guard go(_outputThread);
    
    
    long frameN = -1;
    Mat freezeFrame;
    bool freezed = false;
    bool running = true;
    int key = Keys::NONE;
    while (running && ( _input_channel->isOpen() || !_input_channel->empty()))
    {
        bool hasFrame = true;


        Mat frame, frameOut;
        if (!freezed || key == Keys::n)
        {
            hasFrame = _input_channel->getData(frame);
            freezeFrame = frame;
            frameN++;
        }
        else
        {
            frame = freezeFrame;
        }
        
        
        if (!hasFrame || frame.empty())
        {
            _input_channel->close();
            _output_channel->close();
        }
        else
        {
            if (_showInput && !frame.empty())
                cv::imshow(_inputWindowName, frame);
            auto start_time = chrono::high_resolution_clock::now();

            if (_functor)
                _functor(frameN, frame, frameOut);
            else if (_process)
                _process->operator()(frameN, frame, frameOut);
            
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            
            if (_showTimeInfo)
                printf("I: [%.2f] P: [%.2f] O: [%.2f] \n",
                       _input_channel->getFrequency(),
                       1000.0/double(duration) ,
                       _output_channel->getFrequency());
            
            if (_showOutput && !frameOut.empty())
                cv::imshow(_outputWindowName, frameOut);
            if (_output)
                _output_channel->addData(frameOut);
            
            key = Keys::NONE;

            try
            {
                key = waitKey(1);
            }
            catch (...)
            {
                //...
            }

            if (key == Keys::ESC)
            {
                _input_channel->close();
                _output_channel->close();
                running = false;
            }
            if (key == Keys::SPACE || _pause)
            {
                _pause = false;
                freezeFrame = frame;
                freezed = !freezed;
            }
            if (_kListener && _process && key != Keys::NONE)
                _process->keyboardInput(key);

        }
        
    }
    
    
    destroyAllWindows();
}


void BatchProcessor::mouseCallback(int event, int x, int y, int flags, void *ptr)
{
    MouseListener* listener = (MouseListener*)ptr;
    if (listener)
    {
        listener->mouseInput(event, x, y, flags);
        
        if (event == EVENT_LBUTTONDOWN)
        {
            listener->leftButtonDown(x, y, flags);
        }
        else if (event == EVENT_RBUTTONDOWN)
        {
            listener->rightButtonDown(x, y, flags);
        }
        else if (event == EVENT_MBUTTONDOWN)
        {
            listener->middleButtonDown(x, y, flags);
        }
        else if (event == EVENT_MOUSEMOVE)
        {
            listener->mouseMove(x, y, flags);
        }
    }
}

void BatchProcessor::run()
{
    
    int FLAGS = CV_GUI_NORMAL | CV_WINDOW_AUTOSIZE;
    
    if (_showInput)
        namedWindow(_inputWindowName,FLAGS);
    if (_showOutput)
        namedWindow(_outputWindowName, FLAGS);
    if (_mListener && _batch_process)
        cv::setMouseCallback(_outputWindowName, BatchProcessor::mouseCallback, _batch_process);

    if (!_input && (!_batch_process || !_batch_functor))
        return;
    
    Ptr<BufferedImageChannel> _input_channel = new BufferedImageChannel(_inputBufferSize);
    std::thread  _inputThread(ProcessInput(_input, _input_channel));
    thread_guard gi(_inputThread);
    
    Ptr<BufferedImageChannel> _output_channel = new BufferedImageChannel(_outputBufferSize);
    std::thread  _outputThread(ProcessOutput(_output, _output_channel));
    thread_guard go(_outputThread);
    
    
    size_t frameN = 0;
    size_t numberOfFrames = _batchSize;
    
    vector<Mat> freezeFrames;
    bool freezed = false;

    int key = Keys::NONE;

    while (_input_channel->isOpen())
    {
        
        if (_batch_process && !freezed)
            numberOfFrames = _batch_process->batchProcessSize();
        
        vector<Mat> frames(numberOfFrames);
        
        bool hasFrames = true;
        
        Mat frameOut;
        
        
        if (!freezed)
        {
            for (size_t i = 0; i < numberOfFrames; i++)
            {
                    hasFrames &= _input_channel->getData(frames[i]);
                    hasFrames &= !frames[i].empty();
            }
        }
        else
        {
            frames = freezeFrames;
        }
        
        
        if (!hasFrames)
        {
            _input_channel->close();
            _output_channel->close();
        }
        else
        {
            
            if (_showInput && !frames[numberOfFrames - 1].empty())
                cv::imshow(_inputWindowName, frames[numberOfFrames - 1]);
            auto start_time = chrono::high_resolution_clock::now();
            
            if (_batch_functor)
                _batch_functor(frameN, frames, frameOut);
            else if (_batch_process)
                _batch_process->operator()(frameN, frames, frameOut);
            
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            
            if (_showTimeInfo)
                printf("I: [%.2f] P: [%.2f] O: [%.2f] \n",
                       _input_channel->getFrequency(),
                       1000.0/double(duration) ,
                       _output_channel->getFrequency());
            
            if (_showOutput && !frameOut.empty())
                cv::imshow(_outputWindowName, frameOut);
            if (_output)
                _output_channel->addData(frameOut);

            key = Keys::NONE;

            try
            {
                key = waitKey(1);
            }
            catch (...)
            {
                //...
            }

            if (key == Keys::ESC)
            {
                _input_channel->close();
                _output_channel->close();
            }
            if (key == Keys::SPACE)
            {
                freezeFrames = frames;
                freezed = !freezed;
            }
            if (_kListener && _batch_process && key != Keys::NONE)
                _batch_process->keyboardInput(key);
            
            if (!freezed)
                frameN += numberOfFrames;
        }
        
    }
    
    
    destroyAllWindows();
}


