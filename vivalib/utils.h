/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

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
    struct Keys
    {
        const static int ESC;
		const static int TAB;
		const static int SPACE;
        const static int NONE;
        const static int c;
    };
    
 
    class Files
    {
    public:
        const static string PATH_SEPARATOR;
        
        static string tmpFilenameInFolder(const string &folder = "",
                                          const string &ext = ".jpg");
        static Rect bestSquareFrom(Rect &rectangle);
        
        static void saveSquaredIn(const Mat &image,
                                  string folder,
                                  int side = 200);
        
        static void listdir(const string &dirname, vector<string> &files, bool returnPaths = true);
        static void listImages(const string &dirname, vector<string> &files, bool returnPaths = true);

		static bool isDir(const string &fullpath);
        static bool isFile(const string &fullpath);
		static void makeDir(const string &fullpath);

		static bool exists(const string &fullpath);

		static void getExtension(const string &filename, string &extension);
		static void getFilename(const string &path, string &filename);
        static void getBasename(const string &path, string &base);
    };
    
    
    

}

#endif /* defined(__viva__utils__) */

