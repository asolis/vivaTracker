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

    /**
     * Abstract Input Class defining the interface for a video sequence input type.
     */
    class Input
    {
    protected:
        Size _size;
        bool _convert;
        int _conversionFlag;
		Size _orgSize;
        
    public:
        /**
         *  Input class constructor
         *  @param size: force the input class to an specific size resolution. 
         *               Default value of (-1,-1) will use the original medium resolution.
         *  @param conversionFlag: OpenCV conversion flag e.g., CV_RGB2GRAY. 
         *               Default value of -1 will do not convert the original feed.
         */
        Input(const Size &size = Size(-1, -1),
              int conversionFlag = -1):
                _size(size), _convert(false),
                _conversionFlag(conversionFlag)
        {
            if (_conversionFlag != -1)
                _convert = true;
        }
        /**
         *  Obtain an image frame from the input
         *  @param image: output image from the input.
         *  @return bool: whenever an image was retrieved or not from the input.
         */
        virtual bool  getFrame(Mat &image) = 0;

        /**
         *  Virtual desctructor
         */
        virtual ~Input(){}

        /**
         *  Set the OpenCV conversion flag value e.g., CV_BGR2GRAY. 
         *  Any posterior call to processFrame will return an image
         *  using this conversion type flag.
         */
        void setConversionType(int flag)
        {
            _conversionFlag = flag;
        }

        /**
         * Returns the image's width of the input feed.
         */
        size_t getWidth()
        {
            return _size.width;
        }
        /**
         * Returns the image's height of the input feed
         */
        size_t getHeight()
        {
            return _size.height;
        }
        /**
         * Returns the original input size. 
         * This will be the same to the current image's width and height if 
         * it's created with size(-1,-1).
         */
		Size getOrgSize()
		{
			return _orgSize;
		}
        
    };
    
   
    /**
     *  Base class for Video Sequence inputs
     */
    class VideoInput: public Input
    {
    protected:
        VideoCapture _CameraInput;
        bool _opened;
    public:
        /**
         * VideoInput constructor using a filename.
         * @param filename could be:
         *  1)video file i.e., movie.avi
         *  2)a regular pattern filename i.e. imag0%2d.jpg
         *  3)a url pointing to a video stream i.e., http://domain.com/movie.avi
         * @param size: will scale the original feed to the desired size resolution unless (-1,-1) is defined
         * @param colorFlag: OpenCV conversion flag type value e.g., CV_BGR2GRAY
         *
         */
        VideoInput(const string &filename, const Size &size = Size(-1,-1), int colorFlag = -1);

        /**
         * VideoInput constructor using a camera id.
         * @param id: the camera identifier.
         * @param size: will scale the original feed to the desired size resolution unless (-1,-1) is defined
         * @param colorFlag: OpenCV conversion flag type value e.g., CV_BGR2GRAY
         *
         */
		VideoInput(const int id, const Size &size = Size(-1, -1), int colorFlag = -1);

        /**
         * Default constructor
         */
        VideoInput();

        /**
         * Desctructor
         */
        ~VideoInput();

        /**
         * Overrided from Input Base Class. Used to extract a frame from the input sequence.
         * Returns whether or not a frame was sucessfuly returned.
         * @param frame: output image frame from the sequence
         */
        bool getFrame(Mat &frame);
    };
    
    /**
     * CameraInput class type
     */
    typedef VideoInput CameraInput;

    /**
     * WebStreamInput class type
     */
    typedef VideoInput WebStreamInput;

    /**
     * SequenceInput class type
     */
    typedef VideoInput SequenceInput;
    
    /**
     * ImageListInput to process folder containing images of 
     * supported OpenCV formats.
     */
    class ImageListInput: public Input
    {
    private:
        vector<string>    _filenames;
        vector<string>::iterator _it;
        int _loops;
        bool _opened;
        
        void initialize();

    public:
        /**
         * ImageListInput constructor using a directory path
         */
        ImageListInput(const string directory,
                       const Size &size = Size(-1,-1),
                       int colorFlag = -1,
                       int loops = 1);
        /**
         * ImageListInput constructor using a vector of filenames 
         * containing the images of the sequence in order.
         */
        ImageListInput(const vector<string> &files,
                       const Size &size = Size(-1,-1),
                       int colorFlag = -1,
                       int loops = 1);

        /**
         * Overrided from Input Base Class. Used to extract a frame from the input sequence.
         * Returns whether or not a frame was sucessfuly returned.
         * @param frame: output image frame from the sequence
         */
        bool getFrame(Mat &frame);
    

    };
}





#endif /* defined(__viva__input__) */
