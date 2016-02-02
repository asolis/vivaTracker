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
        bool _convert;
        int  _conversionFlag;
    public:
        Output(const Size &size = Size(-1, -1),
               int  conversionFlag = -1):
                _size(size),
                _convert(false),
                _conversionFlag(conversionFlag)
        {
            if (conversionFlag != -1)
                _convert = true;
        }
        virtual ~Output(){}
        virtual bool  writeFrame(Mat &frame)= 0;
        virtual void  close() {}
    };
    
    //=========================================
    
    class NoneOutput : public Output
    {
    public:
        NoneOutput(const Size &size = Size(-1, -1), int colorFlag = -1):
            Output(size,colorFlag)
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
                    const Size &size = Size(-1, -1),
                    int suffixSize = 5,
                    int  conversionFlag = -1);
        
        virtual bool writeFrame(Mat &frame);
        
    };
    //============================================
    
   
    enum class CODEC : int {
		LIST = CV_FOURCC_PROMPT,
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
                    int codeFlag = -1);
        
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
