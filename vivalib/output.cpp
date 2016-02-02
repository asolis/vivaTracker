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
#include "output.h"


using namespace viva;
ImageOutput::ImageOutput(const string &directory,const Size &size , int suffixSize, int  conversionFlag) :
    Output(size, conversionFlag),_base(directory), _ext(".jpg"),
    _sSize(suffixSize), _internalCount(0), _suffix(0)
{
    if (!Files::exists(_base))
        Files::makeDir(_base);
}

bool ImageOutput::writeFrame(Mat &frame)
{
    std::stringstream ss;
    ss << _base << Files::PATH_SEPARATOR << _suffix <<
        std::setfill('0') << std::setw(_sSize) << _internalCount << _ext;
    if (_convert)
        cvtColor(frame, frame, _conversionFlag);
    if (_size.width > 0 && _size.height > 0)
    {
        resize(frame, frame, _size);
    }
    cv::imwrite(ss.str(), frame);
    _internalCount++;
    return true;

}


void VideoOutput::createOutput()
{
    if (_size.width > 0 && _size.height > 0)
    {
        output.open(_filename, static_cast<int>(_codec), _fps, _size, true);
        _opened = output.isOpened();
        if (!_opened)
        {
            cout << "Video output was not created." << endl;
        }
    }
}

VideoOutput::VideoOutput(const string &filename,
                               Size size,
                               int fps,
                               CODEC codec,
                               int codeFlag ):
    Output(size, codeFlag), _opened(false),
    _fps(fps), _filename(filename), _codec(codec)
{
    createOutput();
}
bool VideoOutput::writeFrame(Mat &frame)
{
	
    
	if (!_opened)
    {
        _size = Size(frame.cols, frame.rows);
        createOutput();
    }
    
    Mat tmp;
	if (_size != frame.size())
		resize(frame, frame, _size);
    if (_convert)
        cvtColor(frame, frame, _conversionFlag);
	
	output << frame;
	return _opened;
}

