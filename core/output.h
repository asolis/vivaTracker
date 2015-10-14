/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __core__output__
#define __core__output__

#include "opencv2/opencv.hpp"
#include <iomanip>
#include "utils.h"

using namespace cv;
using namespace std;

namespace core
{
    class Output
    {
    protected:
        Size _size;
        bool _grayscale;
    public:
        Output(const Size &size = Size(-1, -1), bool grayscale = false):
            _size(size), _grayscale(grayscale)
        {}
        virtual ~Output(){}
        virtual bool  writeFrame(Mat &frame)= 0;
        virtual void  close() {}
    };
    
    //=========================================
    
    class NoneOutput : public Output
    {
    public:
        NoneOutput(const Size &size = Size(-1, -1), bool grayscale = false):
            Output(size,grayscale)
        {}
        bool writeFrame(Mat &frame)
        {
            return true;
        }
    };
    
    //============================================
    
    class ImageOutput: public Output
    {
    private:
        string _base;
        string _ext;
        int    _sSize;
        size_t _internalCount;
        int    _suffix;
        
    public:
        
        ImageOutput(const string &directory,
                    int suffixSize = 5,
                    const Size &size = Size(-1, -1),
                    bool grayscale = false);
        
        virtual bool writeFrame(Mat &frame);
        
    };
    //============================================
    
    class VideoOutput: public Output
    {
        VideoWriter output;
        
        bool   _opened;
        Size   _size;
        int    _fps;
        string _filename;
        
        void createOutput();
        
    public:
        VideoOutput(const string &filename,
                    Size size = Size(-1,-1),
                    int fps = 30);
        
        virtual bool writeFrame(Mat &frame);
        
        ~VideoOutput()
        {
            output.release();
        }
    };
    
   
}


#endif /* defined(__core__output__) */
