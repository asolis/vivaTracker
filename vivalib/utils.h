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
#ifndef __viva__utils__
#define __viva__utils__


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _MSC_VER
	#include "direntVS.h" 
    #include <time.h>
#else
	#include <dirent.h>
#endif

using namespace std;
using namespace cv;

namespace viva
{
    /**
     * Set of custom keys used in the GUI.
     * The ESC key will exit the applicaiton
     * The SPACE will pause and resume the video sequence
     */
    struct Keys
    {
        const static int ESC;
		const static int TAB;
		const static int SPACE;
        const static int NONE;
        const static int n;
        const static int c;
    };
    

    /**
     * Set of static functions related to file I/0 operations
     */
    class Files
    {
    public:

        const static string PATH_SEPARATOR;

        /**
         * Returns a temporary filename inside folder
         */
        static string tmpFilenameInFolder(const string &folder = "",
                                          const string &ext = ".jpg");

        static Rect bestSquareFrom(Rect &rectangle);
        
        static void saveSquaredIn(const Mat &image,
                                  string folder,
                                  int side = 200);

        /**
         * List directory content and returns it in a vector
         */
        static void listdir(const string &dirname, vector<string> &files, bool returnPaths = true);
        /**
         * List a directory content and returns a vector of the images contined in it.
         */
        static void listImages(const string &dirname, vector<string> &files, bool returnPaths = true);

        /**
         * Check if path is a directory
         */
		static bool isDir(const string &fullpath);
        /**
         * Check if path is a regular file
         */
        static bool isFile(const string &fullpath);
        /**
         * Creates a directory in the specified path
         */
		static void makeDir(const string &fullpath);
        /**
         * Checks if the fullpath exits
         */
		static bool exists(const string &fullpath);

        /**
         * Returns the extension of the filename
         */
		static void getExtension(const string &filename, string &extension);
        /**
         * Returns the filename of file in the path
         */
		static void getFilename(const string &path, string &filename);
        /**
         * Returns the basename of file in the path
         */
        static void getBasename(const string &path, string &base);
    };
    
    
    

}

#endif /* defined(__viva__utils__) */

