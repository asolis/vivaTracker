/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __core__h
#define __core__h

#include <thread>

#include "input.h"
#include "processframe.h"
#include "output.h"
#include "channel.h"


using namespace std;
using namespace cv;

namespace core {
    
    class thread_guard
    {
        std::thread& t;
    public:
        explicit thread_guard(std::thread& t_): t(t_)
        {}
        ~thread_guard()
        {
            if(t.joinable())
            {
                t.join();
            }
        }
        thread_guard(thread_guard const&)=delete;
        thread_guard& operator=(thread_guard const&)=delete;
    };
    
    /*
     *  Parallel Processing Input and communicating through the channel
     */
    class ProcessInput
    {
    private:
        Ptr<Input> _input;
        Ptr<BufferedImageChannel> _channel;
  
    public:
        ProcessInput(Ptr<Input> &input,
                     Ptr<BufferedImageChannel> &channel):
            _input(input), _channel(channel)
        {}

        void operator()();
    };
    
    /*
     *  Parallel Processing Output and communicating through the channel
     */
    class ProcessOutput
    {
    private:
        Ptr<Output> _output;
        Ptr<BufferedImageChannel> _channel;

    public:
        ProcessOutput(Ptr<Output> &output,
                      Ptr<BufferedImageChannel> &channel):
            _output(output), _channel(channel)
        {}
   
        void operator()();
    };
    
    
    class Processor
    {
    private:
        Ptr<Input>   _input;
        Ptr<ProcessFrame> _process;
        Ptr<Output>  _output;
        
        string _inputWindowName;
        string _outputWindowName;
        
        bool _showInput;
        bool _showOutput;
        
        bool _mListener;
        bool _kListener;
        
        size_t _inputBufferSize;
        size_t _outputBufferSize;
        
        static void mouseCallback(int event, int x, int y, int flags, void *ptr);
        
    public:
        
        
        Processor(int argc, const char * argv[]) :
        _input(NULL),
        _process(NULL),
        _output(NULL),
        _inputWindowName("Input"),
        _outputWindowName("Process Output"),
        _showInput(false),
        _showOutput(true),
        _mListener(false),
        _kListener(false)
        {
          
        }
        void setInputBufferSize(size_t size)
        {
            _inputBufferSize = size;
        }
        
        void setOutputBufferSize(size_t size)
        {
            _outputBufferSize = size;
        }
        
        void showInput(bool show = true)
        {
            _showInput = show;
        }
        void showOutput(bool show = true)
        {
            _showOutput = show;
        }
        void setInputWindowName(const string &name)
        {
            _inputWindowName = name;
        }
        void setOutputWindowName(const string &name)
        {
            _outputWindowName = name;
        }
        void setInput(Ptr<Input> &input)
        {
            _input = input;
        }
        void setOutput(Ptr<Output> &output)
        {
            _output = output;
        }
        void addMouseListener()
        {
            _mListener = true;
        }
        void addKeyboardListener()
        {
            _kListener = true;
        }
        void setProcess(Ptr<ProcessFrame> &process)
        {
            _process = process;
        }
        void run();
};
    
    
}

#endif
