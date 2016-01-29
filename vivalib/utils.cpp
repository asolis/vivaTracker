/*******************************************************
 * Copyright (C) 2015 Andrés Solís Montero <andres@solism.ca>
 *   PhD Candidate.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 *******************************************************/

#include "utils.h"

using namespace viva;

const int Keys::ESC   = 27;
const int Keys::TAB   = 9;
const int Keys::SPACE = 32;
const int Keys::NONE  = -1;
const int Keys::c = 'c';

string Files::tmpFilenameInFolder(const string &folder,
                                  const string &ext)
{
    stringstream ss;
    time_t  timev; tm * timeinfo;
    time(&timev); timeinfo = localtime(&timev);
    char buffer[13];
    strftime(buffer, 13, "%y%m%d%H%M%S\0", timeinfo);
    ss << folder << buffer << ext;
    return ss.str();
}

Rect Files::bestSquareFrom(Rect &rectangle)
{
    Point2f center(rectangle.x + rectangle.width / 2,
                   rectangle.y + rectangle.height / 2);
    int side = min(rectangle.width, rectangle.height);
    Point2f tl(center.x - side/2, center.y - side/2);
    return Rect(tl.x, tl.y, side, side);
}

void Files::saveSquaredIn(const Mat &image, string folder, int side)
{
    
    Mat ROI;
    if (image.rows != image.cols)
    {
        Rect rImage(0,0, image.cols, image.rows);
        Rect rFinal = Files::bestSquareFrom(rImage);
        ROI = image(rFinal).clone();
    }
    else
        image.copyTo(ROI);
    imwrite(Files::tmpFilenameInFolder(folder), ROI);
    
}


const string Files::PATH_SEPARATOR =
#ifdef _WIN32
"\\";
#else
"/";
#endif

void Files::listdir(const string &dirname, vector<string> &files, bool returnPaths)
{
    DIR *dp;
    dirent *d;
    
    dp = opendir(dirname.c_str());
    
    while((d = readdir(dp)) != NULL)
    {
        // ignore the special directories
        if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
        {
            string ret;
            if (returnPaths)
                ret = dirname + Files::PATH_SEPARATOR + d->d_name;
            else
                ret = d->d_name;
            
            files.push_back(ret);
        }
    }
    
    sort(files.begin(), files.end());
}

void Files::listImages(const string &dirname, vector<string> &files, bool returnPaths)
{
    DIR *dp;
    dirent *d;
    
    dp = opendir(dirname.c_str());
    
    vector<string> exts = {".jpg", ".jpeg", ".png", ".bmp", ".JPG", ".JPEG", ".PNG", ".BMP"};
    
    while((d = readdir(dp)) != NULL)
    {
        string name(d->d_name);
        size_t lDotPos = name.find_last_of(".");

        if (lDotPos != string::npos)
        {
            string ext = name.substr(lDotPos);
            if (find(exts.begin(), exts.end(), ext) != exts.end())
            {
                string ret;
                if (returnPaths)
                    ret = dirname + Files::PATH_SEPARATOR + d->d_name;
                else
                    ret = d->d_name;
                
                files.push_back(ret);
            }
        }

        
    }
    
    sort(files.begin(), files.end());
}
bool Files::isFile(const string &fullpath)
{
    struct stat st;
    stat(fullpath.c_str(), &st);
    if(S_ISREG(st.st_mode))
        return true;
    else
        return false;
}

bool Files::isDir(const string &fullpath)
{
    struct stat st;
    stat(fullpath.c_str(), &st);
    if(S_ISDIR(st.st_mode))
        return true;
    else
        return false;
}
        
void Files::makeDir(const string &fullpath)
{
    if (!exists(fullpath))
    {
#ifdef _WIN32_WINNT
        wstring wrapper(fullpath.begin(), fullpath.end());
        _wmkdir(wrapper.c_str());
#else
        mkdir(fullpath.c_str(), 0777);
#endif
    }
}

bool Files::exists(const string &fullpath)
{
    struct stat buffer;
    return (stat (fullpath.c_str(), &buffer) == 0);
}

void Files::getExtension(const string &filename, string &extension)
{
    size_t index = filename.find_last_of(".");
    extension = filename.substr(index + 1);
}

void Files::getBasename(const string &path, string &base)
{
    size_t index = path.find_last_of("/\\");
    base = path.substr(0, index + 1);
}

void Files::getFilename(const string &path, string &filename)
{
    size_t index = path.find_last_of("/\\");
    filename = path.substr(index + 1);
}



