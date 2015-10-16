/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#ifndef __viva__output__
#define __viva__output__

#include "opencv2/opencv.hpp"
#include <iomanip>
#include "utils.h"

using namespace cv;
using namespace std;

namespace viva
{
    class Output
    {
    protected:
        Size _size;
        bool _grayscale;
        int  _conversionFlag;
    public:
        Output(const Size &size = Size(-1, -1),
               bool grayscale = false,
               int  conversionFlag = CV_RGB2GRAY):
                _size(size),
                _grayscale(grayscale),
                _conversionFlag(conversionFlag)
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
                    bool grayscale = false,
                    int  conversionFlag = CV_RGB2GRAY);
        
        virtual bool writeFrame(Mat &frame);
        
    };
    //============================================
    
   
    enum class CODEC : int {
        LIST = -1,
        MPG1  = CV_FOURCC_MACRO('P','I','M','1'),     //MPEG-1
        MJPEG = CV_FOURCC_MACRO('M','J','P','G'),     //Motion-jpeg
        MPEG42= CV_FOURCC_MACRO('M', 'P', '4', '2'),  //MPEG-4.2
        MPEG43= CV_FOURCC_MACRO('D', 'I', 'V', '3'),  //MPEG-4.3
        MPEG4 = CV_FOURCC_MACRO('D', 'I', 'V', 'X'),  //MPEG-4
        H263  = CV_FOURCC_MACRO('U', '2', '6', '3'),  //H263
        H263I = CV_FOURCC_MACRO('I', '2', '6', '3'),  //H263I
        FLV1  = CV_FOURCC_MACRO('F', 'L', 'V', '1'),  //FLV1
        H264  = CV_FOURCC_MACRO('X','2','6','4')      //H.264
    };

    class VideoOutput: public Output
    {
        VideoWriter output;
        
        bool   _opened;
        int    _fps;
        string _filename;
        CODEC  _codec;
        void createOutput();
        
    public:
        VideoOutput(const string &filename,
                    Size size   = Size(-1,-1),
                    int fps     = 30,
                    CODEC codec = CODEC::MPEG4,
                    bool grayscale = false,
                    int  conversionFlag = CV_RGB2GRAY);
        
        virtual bool writeFrame(Mat &frame);
        
        void setCodec(CODEC codec)
        {
            _codec = codec;
        }
        
        
        ~VideoOutput()
        {
            output.release();
        }
    };
    
   
}


#endif /* defined(__viva__output__) */
