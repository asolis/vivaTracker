/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "input.h"

using namespace viva;

CameraInput::CameraInput(int device, const Size &size, bool grayscale ):
    Input(size, grayscale)
{
    _CameraInput = cv::VideoCapture(device);
    if (_size.width > 0 && _size.height > 0)
    {
        _CameraInput.set(CV_CAP_PROP_FRAME_WIDTH, _size.width);
        _CameraInput.set(CV_CAP_PROP_FRAME_HEIGHT, _size.height);
    }
    _size.width = _CameraInput.get(CV_CAP_PROP_FRAME_WIDTH);
    _size.height = _CameraInput.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    _opened = _CameraInput.isOpened();
}
CameraInput::~CameraInput()
{
    _CameraInput.release();
}

bool CameraInput::getFrame(Mat &frame)
{
    if (!_opened || !_CameraInput.read(frame))
    {
        _opened = false;
        return false;
    }
    if (frame.channels() > 1 && _grayscale)
        cvtColor(frame, frame, _conversionFlag);
    
    return true;
}

VideoInput::VideoInput(const string &filename, const Size &size, bool grayscale ):
    Input(size, grayscale)
{
    _CameraInput = cv::VideoCapture(filename);
    if (_size.width > 0 && _size.height > 0)
    {
        _CameraInput.set(CV_CAP_PROP_FRAME_WIDTH, _size.width);
        _CameraInput.set(CV_CAP_PROP_FRAME_HEIGHT, _size.height);
    }
    _size.width = _CameraInput.get(CV_CAP_PROP_FRAME_WIDTH);
    _size.height = _CameraInput.get(CV_CAP_PROP_FRAME_HEIGHT);
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
    if (!_opened || !_CameraInput.read(frame))
    {
        _opened = false;
        return false;
    }
    if (frame.channels() > 1 && _grayscale)
        cvtColor(frame, frame, _conversionFlag);
    
    return true;
}





ImageListInput::ImageListInput(const string directory, const Size &size, bool grayscale, int loops ):
Input(Size(-1,-1), grayscale), _loops(loops)
{
    Files::listImages(directory, _filenames);
    initialize();
}
ImageListInput::ImageListInput(const vector<string> &files, const Size &size , bool grayscale  ,int loops ):
Input(size, grayscale), _filenames(files), _loops(loops)
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
        if (_grayscale)
            frame = imread(*_it,CV_LOAD_IMAGE_GRAYSCALE);
        else
            frame = imread(*_it);
        
        if (_size.width > 0 && _size.height > 0)
        {
            resize(frame, frame, _size);
        }
        _size.width  = frame.cols;
        _size.height = frame.rows;
        _it++;
        return true;
    }
    return false;
}

