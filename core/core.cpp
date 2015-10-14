/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "core.h"


void core::ProcessInput::operator()()
{
    if (!_input)
    {
        _channel->close();
        return;
    }
    while (_channel->isOpen())
    {
        Mat frame;
        bool hasFrame = _input->getFrame(frame);
        if (!hasFrame || frame.empty())
            _channel->close();
        else
        {
            _channel->addImage(frame);
        }
        
    }
}

void core::ProcessOutput::operator()()
{
    if (!_output)
    {
        _channel->close();
        return;
    }
    while (_channel->isOpen())
    {
        Mat frame;
        bool hasFrame = _channel->getFrame(frame);
        if (!hasFrame || frame.empty())
            _channel->close();
        else
        {
            _output->writeFrame(frame);
        }
        
    }
}

void core::Processor::mouseCallback(int event, int x, int y, int flags, void *ptr)
{
    MouseListener* listener = (MouseListener*)ptr;
    if (listener != NULL)
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


void core::Processor::run()
{
    
    int FLAGS = CV_GUI_NORMAL | CV_WINDOW_AUTOSIZE;
    
    if (_showInput)
        namedWindow(_inputWindowName,FLAGS);
    if (_showOutput)
        namedWindow(_outputWindowName, FLAGS);
    if (_mListener)
        cv::setMouseCallback(_outputWindowName, Processor::mouseCallback, _process);
    
    if (!_input || !_process)
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
        Mat frame, frameOut;
        bool hasFrame = true;
        
        if (!freezed)
            hasFrame = _input_channel->getFrame(frame);
        else
            frame = freezeFrame;
        
        
        if (!hasFrame || frame.empty())
        {
            _input_channel->close();
            _output_channel->close();
        }
        else
        {
            if (_showInput && !frame.empty())
                cv::imshow(_inputWindowName, frame);
            _process->execute(frameN, frame, frameOut);
            if (_showOutput && !frameOut.empty())
                cv::imshow(_outputWindowName, frameOut);
            if (_output)
                _output_channel->addImage(frameOut);
            
            int key = waitKey(1);
            if (key == core::Keys::ESC)
            {
                _input_channel->close();
                _output_channel->close();
            }
            if (key == core::Keys::SPACE)
            {
                freezeFrame = frame;
                freezed = !freezed;
            }
            if (_kListener && key != core::Keys::NONE)
                _process->keyboardInput(key);
            
            if (!freezed)
                frameN++;
        }
        
    }
    
    
    destroyAllWindows();
}