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
