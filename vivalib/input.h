/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __viva__input__
#define __viva__input__

#include "opencv2/opencv.hpp"
#include "utils.h"
#include <vector>

using namespace cv;
using namespace std;

namespace viva
{

    class Input
    {
    protected:
        Size _size;
        bool _convert;
        int _conversionFlag;
		Size _orgSize;
        
    public:
        Input(const Size &size = Size(-1, -1),
              int conversionFlag = -1):
                _size(size), _convert(false),
                _conversionFlag(conversionFlag)
        {
            if (_conversionFlag != -1)
                _convert = true;
        }
        
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
		Size getOrgSize()
		{
			return _orgSize;
		}
        
    };
    
   
    
    class VideoInput: public Input
    {
    protected:
        VideoCapture _CameraInput;
        bool _opened;
    public:
        
        VideoInput(const string &filename, const Size &size = Size(-1,-1), int colorFlag = -1);
		VideoInput(const int id, const Size &size = Size(-1, -1), int colorFlag = -1);
        VideoInput();
        ~VideoInput();
        bool getFrame(Mat &frame);
    };
    
    
    typedef VideoInput CameraInput;
    typedef VideoInput WebStreamInput;
    typedef VideoInput SequenceInput;
    
    
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
                       int colorFlag = -1,
                       int loops = 1);
        ImageListInput(const vector<string> &files,
                       const Size &size = Size(-1,-1),
                       int colorFlag = -1,
                       int loops = 1);
        
        bool getFrame(Mat &frame);
    

    };
}





#endif /* defined(__viva__input__) */
