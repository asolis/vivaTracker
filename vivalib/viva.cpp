/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

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
            _channel->close();
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
    
    
    size_t frameN = 0;
    Mat freezeFrame;
    bool freezed = false;
    while (_input_channel->isOpen())
    {
        
        bool hasFrame = true;
        
        Mat frame, frameOut;
        
      
        
        if (!freezed)
        {
            hasFrame = _input_channel->getData(frame);
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
            
            int key = waitKey(1);
            if (key == Keys::ESC)
            {
                _input_channel->close();
                _output_channel->close();
            }
            if (key == Keys::SPACE)
            {
                freezeFrame = frame;
                freezed = !freezed;
            }
            if (_kListener && _process && key != Keys::NONE)
                _process->keyboardInput(key);
            
            if (!freezed)
                frameN++;
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
            
            
            
            int key = waitKey(1);
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


