/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "input.h"

using namespace viva;

VideoInput::VideoInput(const int device, const Size &size, int colorFlag) :
Input(size, colorFlag)
{
    _CameraInput.open(device);
    if (_size.width > 0 && _size.height > 0)
    {
        _CameraInput.set(CV_CAP_PROP_FRAME_WIDTH, _size.width);
        _CameraInput.set(CV_CAP_PROP_FRAME_HEIGHT, _size.height);
    }
    
    _opened = _CameraInput.isOpened();
}



VideoInput::VideoInput(const string &filename, const Size &size, int colorFlag ):
    Input(size, colorFlag)
{
    _CameraInput.open(filename);
    if (_size.width > 0 && _size.height > 0)
    {
        _CameraInput.set(CV_CAP_PROP_FRAME_WIDTH, _size.width);
        _CameraInput.set(CV_CAP_PROP_FRAME_HEIGHT, _size.height);
    }
	
	_opened = _CameraInput.isOpened();
}
VideoInput::VideoInput()
{
    _CameraInput.release();
    _opened = false;
}
VideoInput::~VideoInput()
{
    _CameraInput.release();
}
bool VideoInput::getFrame(Mat &frame)
{

    if (!_opened || !_CameraInput.grab())
    {
        _opened = false;
        return false;
    }
	_CameraInput.retrieve(frame);
	_orgSize = frame.size();
	if (_size.width < 0 && _size.height < 0)
	{

	}
	else if (_size.width < 0 && _size.height > 0)
	{
		resize(frame, frame, Size(_orgSize.width*_size.height / _orgSize.height, _size.height));
	}
	else if (_size.width > 0 && _size.height < 0)
	{
		resize(frame, frame, Size(_size.width, _orgSize.height * _size.width / _orgSize.width));
	}
	else if (_size.width != frame.cols && _size.height != frame.rows)
	{
		resize(frame, frame, _size);
	}
	if (_convert)
		cvtColor(frame, frame, _conversionFlag);


       
    
    return true;
}


ImageListInput::ImageListInput(const string directory, const Size &size, int colorFlag, int loops ):
Input(size, colorFlag), _loops(loops)
{
    Files::listImages(directory, _filenames);
    initialize();
}
ImageListInput::ImageListInput(const vector<string> &files, const Size &size , int colorFlag  ,int loops ):
Input(size, colorFlag), _filenames(files), _loops(loops)
{
    initialize();
}

void ImageListInput::initialize()
{
    if (_filenames.size() < 1)
    {
        _it = _filenames.end();
        _opened = false;
        return;
    }
    _it = _filenames.begin();
    _opened = true;
}

bool ImageListInput::getFrame(Mat &frame)
{
    if (_it == _filenames.end() && (_loops > 0 || _loops < 0))
    {
        _it = _filenames.begin();
        _loops--;
    }
    if (_it!= _filenames.end())
    {
        frame = imread(*_it);

		_orgSize = frame.size();
		if (_size.width < 0 && _size.height < 0)
		{

		}
		else if (_size.width < 0 && _size.height > 0)
		{
			resize(frame, frame, Size(_orgSize.width*_size.height / _orgSize.height, _size.height));
		}
		else if (_size.width > 0 && _size.height < 0)
		{
			resize(frame, frame, Size(_size.width, _orgSize.height * _size.width / _orgSize.width));
		}
		else if (_size.width != frame.cols && _size.height != frame.rows)
        {
            resize(frame, frame, _size);
        }
        if (_convert)
            cvtColor(frame, frame, _conversionFlag);
        
        _size.width  = frame.cols;
        _size.height = frame.rows;
        _it++;
        return true;
    }
    return false;
}

