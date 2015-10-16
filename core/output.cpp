/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "output.h"

core::ImageOutput::ImageOutput(const string &directory, int suffixSize,
                               const Size &size ,
                               bool grayscale,
                               int  conversionFlag) :
    Output(size, grayscale, conversionFlag),_base(directory), _ext(".jpg"),
    _sSize(suffixSize), _internalCount(0), _suffix(0)
{
    if (!core::Files::exists(_base))
        core::Files::makeDir(_base);
}

bool core::ImageOutput::writeFrame(Mat &frame)
{
    std::stringstream ss;
    ss << _base << core::Files::PATH_SEPARATOR << _suffix <<
        std::setfill('0') << std::setw(_sSize) << _internalCount << _ext;
    if (_grayscale && (frame.channels() == 3))
        cvtColor(frame, frame, _conversionFlag);
    if (_size.width > 0 && _size.height > 0)
    {
        resize(frame, frame, _size);
    }
    cv::imwrite(ss.str(), frame);
    _internalCount++;
    return true;

}


void core::VideoOutput::createOutput()
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

core::VideoOutput::VideoOutput(const string &filename,
                               Size size,
                               int fps,
                               CODEC codec,
                               bool grayscale,
                               int  conversionFlag ):
    Output(size, grayscale, conversionFlag), _opened(false),
    _fps(fps), _filename(filename), _codec(codec)
{
    createOutput();
}
bool core::VideoOutput::writeFrame(Mat &frame)
{
	
    
	if (!_opened)
    {
        _size = Size(frame.cols, frame.rows);
        createOutput();
    }
    
    Mat tmp;
	if (_size != frame.size())
		resize(frame, frame, _size);
    if (_grayscale)
        cvtColor(frame, frame, _conversionFlag);
	
	output << frame;
	return _opened;
}

