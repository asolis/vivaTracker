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

    /**
     * Thread safe guard created to ensure all threads are joing 
     * when exiting their scope.
     */
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
    
    /**
     * ProcessFrame functor interface accepting mouse and keyboard events
     */
    class ProcessFrame : public MouseListener, public KeyboardListener
    {
    public:
        virtual ~ProcessFrame(){}
        virtual void operator()(const size_t frameN, const Mat &frame, Mat &output)
        {
            output = frame.clone();
        };
        
        /**
         *Inherited from MouseListener. Check MouseListener class for details
         */
        virtual void mouseInput(int event, int x, int y, int flags){};
        /**
         *Inherited from MouseListener. Check MouseListener class for details
         */
        virtual void leftButtonDown(int x, int y, int flags){};
        /**
         *Inherited from MouseListener. Check MouseListener class for details
         */
        virtual void rightButtonDown(int x, int y, int flags){};
        /**
         *Inherited from MouseListener. Check MouseListener class for details
         */
        virtual void middleButtonDown(int x, int y, int flags){};
        /**
         *Inherited from MouseListener. Check MouseListener class for details
         */
        virtual void mouseMove(int x, int y, int flags){};
        
        /**
         * Inherited from KeyboardListerner. Check KeyboardListener class for details.
         */
        virtual void keyboardInput(int key){};
        
    };
    
    /**
     * Batch Process Frame. Process batchs of specific size from the input at once.
     */
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
    
    
    /**
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
    
    /**
     * Processor class. 
     * Entry point of a vivalib program
     * We need to define an Input , ProcessFrame and Output(optional) before starting to run a processor
     */
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
        bool _pause;
        
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
        _showTimeInfo(false),
        _pause(false)
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
        /**
         * Makes to pause the sequence at the fist frame. Waiting for the user 
         * to hit the SPACE key in the keyboard to continue the execution of the sequence.
         * It's usefull when manual user selection is needed before running the algorithm
         */
        void startPaused()
        {
            _pause = true;
        }
        
        /**
         * Set a process
         */
        void setProcess(function<void(const size_t frameN,const Mat &frame, Mat &output)> functor)
        {
            _functor = functor;
        }

        /**
         * Method to execute after defining the Processor's Input, ProcessFrame, and Output(optional)
         * The input, process and output will run in three different threads and 
         * will comunicate using BuffedChannels between them.
         * To exit you should press the ESC key. The SPACE key will allow to pause and/or continue the video
         * sequence
         */
        void run();
    };

    /**
     * Similar to Processor but instead of processing one image a the time.
     * it waits to have a buffer of images to precess them at once.
     */
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
        /**
         * Set an input
         */
        void setInput(Ptr<Input> &input)
        {
            _input = input;
        }
        /**
         * Set an output
         */
        void setOutput(Ptr<Output> &output)
        {
            _output = output;
        }
        /**
         * Make the processor to listen mouse events
         */
        void listenToMouseEvents()
        {
            _mListener = true;
        }
        /**
         * Make the processor to listen to keyboard events
         */
        void listenToKeyboardEvents()
        {
            _kListener = true;
        }

        /**
         * Set a batch process
         */
        void setBatchProcess(Ptr<BatchProcessFrame> &process)
        {
            _batch_process = process;
        }

        /**
         * Set a batch process
         */
        void setBatchProcess(function<void(const size_t frameN, const vector<Mat> &frames, Mat &output)> functor)
        {
            _batch_functor = functor;
        }

        /**
         * Method to run once the input , processframe, and output (optional) are set
         */
        void run();
    };
    
    
    
    
 
}


#endif
