/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __core__input__
#define __core__input__

#include "opencv2/opencv.hpp"
#include "utils.h"
#include <vector>

using namespace cv;
using namespace std;

namespace core
{

    class Input
    {
    protected:
        bool _grayscale;
        Size _size;
        int _conversionFlag;
        
    public:
        Input(const Size &size = Size(-1, -1),
              bool grayscale   = false):
            _grayscale(grayscale), _size(size), _conversionFlag(CV_RGB2GRAY){}
        

        virtual bool  getFrame(Mat &image) = 0;
        virtual ~Input(){}
        
        void setConversionType(int flag)
        {
            _conversionFlag = flag;
        }
        size_t getWidth()
        {
            return _size.width;
        }
        size_t getHeight()
        {
            return _size.height;
        }
        
    };
    
    class CameraInput: public Input
    {
    private:
        VideoCapture _CameraInput;
        bool _opened;
    public:
        CameraInput(int device = 0, const Size &size = Size(-1,-1), bool grayscale = false);
        ~CameraInput();
        bool getFrame(Mat &frame);
        
    };
    
    class VideoInput: public Input
    {
    private:
        VideoCapture _CameraInput;
        bool _opened;
    public:
        
        VideoInput(const string &filename, const Size &size = Size(-1,-1), bool grayscale = false);
        VideoInput();
        ~VideoInput();
        bool getFrame(Mat &frame);
    };
    
    
    class ImageListInput: public Input
    {
    private:
        vector<string>    _filenames;
        vector<string>::iterator _it;
        int _loops;
        bool _opened;
        
        void initialize();

    public:
        ImageListInput(const string directory,
                       const Size &size = Size(-1,-1),
                       bool grayscale = false,
                       int loops = 1);
        ImageListInput(const vector<string> &files,
                       const Size &size = Size(-1,-1),
                       bool grayscale = false ,
                       int loops = 1);
        
        bool getFrame(Mat &frame);
    

    };
}





#endif /* defined(__core__input__) */
