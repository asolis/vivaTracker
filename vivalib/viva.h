/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __viva__h
#define __viva__h

#include <thread>
#include <functional>
#include <vector>

#include "input.h"
#include "listener.h"
#include "output.h"
#include "channel.h"


using namespace std;
using namespace cv;

namespace viva {
    
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
    
    
    class ProcessFrame : public MouseListener, public KeyboardListener
    {
    public:
        virtual ~ProcessFrame(){}
        virtual void operator()(const size_t frameN, const Mat &frame, Mat &output)
        {
            output = frame.clone();
        };
        
        //Inherited from MouseListener. Check MouseListener class for details
        virtual void mouseInput(int event, int x, int y, int flags){};
        virtual void leftButtonDown(int x, int y, int flags){};
        virtual void rightButtonDown(int x, int y, int flags){};
        virtual void middleButtonDown(int x, int y, int flags){};
        virtual void mouseMove(int x, int y, int flags){};
        
        //Inherited from KeyboardListerner. Check KeyboardListener class for details.
        virtual void keyboardInput(int key){};
        
    };
    
    
    class BatchProcessFrame : public MouseListener, public KeyboardListener
    {
    protected:
        size_t _count;
    public:
        BatchProcessFrame(const size_t count = 10): _count(count){}
        virtual ~BatchProcessFrame(){}
        
        virtual size_t batchProcessSize()
        {
            return _count;
        }
        
        virtual void operator()(size_t frameN, const vector<Mat> &frames, Mat &output)
        {
            output = frames[0].clone();
        };
        
        //Inherited from MouseListener. Check MouseListener class for details
        virtual void mouseInput(int event, int x, int y, int flags){};
        virtual void leftButtonDown(int x, int y, int flags){};
        virtual void rightButtonDown(int x, int y, int flags){};
        virtual void middleButtonDown(int x, int y, int flags){};
        virtual void mouseMove(int x, int y, int flags){};
        
        //Inherited from KeyboardListerner. Check KeyboardListener class for details.
        virtual void keyboardInput(int key){};
        
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
        
        function<void(const size_t frameN, const Mat &frame, Mat &output)> _functor;
        
        string _inputWindowName;
        string _outputWindowName;
        
        bool _showInput;
        bool _showOutput;
        
        bool _mListener;
        bool _kListener;
        
        size_t _inputBufferSize;
        size_t _outputBufferSize;
        
        bool _showTimeInfo;
        
        static void mouseCallback(int event, int x, int y, int flags, void *ptr);
        
    public:
        
        Processor():
        _input(new CameraInput(0)),
        _process(new ProcessFrame()),
        _output(Ptr<Output>()),
        _functor(nullptr),
        _inputWindowName("Input"),
        _outputWindowName("Process Output"),
        _showInput(false),
        _showOutput(true),
        _mListener(false),
        _kListener(false),
        _inputBufferSize(10),
        _outputBufferSize(10),
        _showTimeInfo(false)
        {}
        
        Processor(int argc, const char * argv[]) : Processor()
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
        void showTimeInfo(bool show = true)
        {
            _showTimeInfo = show;
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
        void listenToMouseEvents()
        {
            _mListener = true;
        }
        void listenToKeyboardEvents()
        {
            _kListener = true;
        }
        void setProcess(Ptr<ProcessFrame> &process)
        {
            _process = process;
        }
        
        void setProcess(function<void(const size_t frameN,const Mat &frame, Mat &output)> functor)
        {
            _functor = functor;
        }

        
        void run();
    };
    
    class BatchProcessor
    {
    private:
        size_t _batchSize;
        Ptr<Input>   _input;
        Ptr<BatchProcessFrame> _batch_process;
        Ptr<Output>  _output;
        
        function<void(const size_t frameN, const vector<Mat> &frames, Mat &output)> _batch_functor;
        
        string _inputWindowName;
        string _outputWindowName;
        
        bool _showInput;
        bool _showOutput;
        
        bool _mListener;
        bool _kListener;
        
        size_t _inputBufferSize;
        size_t _outputBufferSize;
        
        bool _showTimeInfo;
        
        static void mouseCallback(int event, int x, int y, int flags, void *ptr);
        
    public:
        
        BatchProcessor(size_t batchSize = 10):
        _batchSize(batchSize),
        _input(new CameraInput(0)),
        _batch_process(new BatchProcessFrame(_batchSize)),
        _output(Ptr<Output>()),
        _batch_functor(nullptr),
        _inputWindowName("Input"),
        _outputWindowName("Process Output"),
        _showInput(false),
        _showOutput(true),
        _mListener(false),
        _kListener(false),
        _inputBufferSize(10),
        _outputBufferSize(10),
        _showTimeInfo(false)
        {}
        
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
        void showTimeInfo(bool show = true)
        {
            _showTimeInfo = show;
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
        void listenToMouseEvents()
        {
            _mListener = true;
        }
        void listenToKeyboardEvents()
        {
            _kListener = true;
        }

        void setBatchProcess(Ptr<BatchProcessFrame> &process)
        {
            _batch_process = process;
        }
        
        void setBatchProcess(function<void(const size_t frameN, const vector<Mat> &frames, Mat &output)> functor)
        {
            _batch_functor = functor;
        }
        
        void run();
    };
    
    
    
    
 
}


#endif
