/**************************************************************************************************
 **************************************************************************************************

 GPL-3 License (https://www.tldrlegal.com/l/gpl-3.0)

 Copyright (c) 2015 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.

 sKCF is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 **************************************************************************************************
 **************************************************************************************************/
#ifndef _GRADIENT_H_
#define _GRADIENT_H_


#define _IDX_COLUMN_MAJOR(r,c,k,nrows,ncols)        (r + (c + k * ncols) * nrows)
#define _IDX_ROW_MAJOR(r,c,k,nrows,ncols,nchannels) ((r * ncols + c) * nchannels + k)

#define _IDX_COLUMN_MAJOR_C1(r,c,nrows,ncols)     (r + c * nrows)
#define _IDX_ROW_MAJOR_C1(r,c,nrows,ncols)        (r * ncols + c)

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

template <typename T> void OpenCV2Matlab(const cv::Mat& from, T* to)
{
    assert((from.dims == 2) && from.isContinuous());
    const int channels= from.channels();
    const int rows    = from.rows;
    const int cols    = from.cols;
    const T* pdata = (T*)from.data;
    
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            for (size_t k = 0; k < channels; k++)
            {
                to[_IDX_COLUMN_MAJOR(r, c, k, rows, cols)] =
                pdata[_IDX_ROW_MAJOR(r,c,k,rows,cols,channels)];
            }
}
template <typename T> void Matlab2OpenCV(const T* from, cv::Mat& to)
{
    assert((to.dims == 2) && to.isContinuous());
    const int channels= to.channels();
    const int rows    = to.rows;
    const int cols    = to.cols;
    
    T* pdata = (T*)to.data;
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            for (size_t k = 0; k < channels; k++)
            {
                pdata[_IDX_ROW_MAJOR(r,c,k,rows,cols,channels)] =
                from[_IDX_COLUMN_MAJOR(r, c,k,rows,cols)];
            }
}
template <typename T> void OpenCV2MatlabCN(const cv::Mat& from, T* to,const int* order)
{
    assert((from.dims == 2) && from.isContinuous());
    const int channels= from.channels();
    const int rows    = from.rows;
    const int cols    = from.cols;
    const T* pdata = (T*)from.data;
    
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            for (size_t k = 0; k < channels; k++)
            {
                to[_IDX_COLUMN_MAJOR(r, c, k, rows, cols)] = pdata[_IDX_ROW_MAJOR(r,c,order[k],rows,cols,channels)];
            }
}
template <typename T> void Matlab2OpenCVCN(const T* from, cv::Mat& to,const int* order)
{
    assert((to.dims == 2) && to.isContinuous());
    const int channels= to.channels();
    const int rows    = to.rows;
    const int cols    = to.cols;
    
    T* pdata = (T*)to.data;
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            for (size_t k = 0; k < channels; k++)
            {
                pdata[_IDX_ROW_MAJOR(r,c,k,rows,cols,channels)] =
                from[_IDX_COLUMN_MAJOR(r, c,order[k],rows,cols)];
            }
}
template <typename T> void OpenCV2MatlabC1(const cv::Mat& from, T* to)
{
    assert((from.dims == 2) && from.isContinuous() && (from.channels() == 1));
    const int rows    = from.rows;
    const int cols    = from.cols;
    const T* pdata = (T*)from.data;
    
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
        {
            to[_IDX_COLUMN_MAJOR_C1(r,c,rows,cols)] =
            pdata[_IDX_ROW_MAJOR_C1(r,c,rows,cols)];
        }
}
template <typename T> void Matlab2OpenCVC1(const T* from, cv::Mat& to)
{
    assert((to.dims == 2) && to.isContinuous() && (to.channels() == 1));
    const int rows    = to.rows;
    const int cols    = to.cols;
    
    T* pdata = (T*)to.data;
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
        {
            pdata[_IDX_ROW_MAJOR_C1(r,c,rows,cols)] =
            from[_IDX_COLUMN_MAJOR_C1(r,c,rows, cols)];
        }
}
template <typename T> void OpenCVBGR_MatlabRGB(const cv::Mat& from, T* to)
{
    assert((from.dims == 2) && from.isContinuous() && (from.channels() == 3));
    const int rows    = from.rows;
    const int cols    = from.cols;
    const T* pdata = (T*)from.data;
    
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
        {
            size_t  tmp = _IDX_ROW_MAJOR(r,c,0,rows,cols,3);
            to[_IDX_COLUMN_MAJOR(r, c, 0, rows, cols)] = pdata[tmp + 2]; // R  B
            to[_IDX_COLUMN_MAJOR(r, c, 1, rows, cols)] = pdata[tmp + 1]; // G  G
            to[_IDX_COLUMN_MAJOR(r, c, 2, rows, cols)] = pdata[tmp];     // B  R
        }
}
template <typename T> void MatlabRGB_OpenCVBGR(const T* from, cv::Mat& to)
{
    assert((to.dims == 2) && to.isContinuous() && (to.channels() == 1));
    const int rows    = to.rows;
    const int cols    = to.cols;
    
    T* pdata = (T*)to.data;
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
        {
            size_t  tmp = _IDX_ROW_MAJOR(r,c,0,rows,cols,3);
            pdata[tmp]     = from[_IDX_COLUMN_MAJOR(r, c, 2,rows,cols)];//B R
            pdata[tmp + 1] = from[_IDX_COLUMN_MAJOR(r, c, 1,rows,cols)];//G G
            pdata[tmp + 2] = from[_IDX_COLUMN_MAJOR(r, c, 0,rows,cols)];//R B
        }
}

//% INPUTS
//%  I          - [hxwxk] input k channel single image
//% OUTPUTS
//%  M          - [hxw] gradient magnitude at each location
//%  O          - [hxw] approximate gradient orientation modulo PI
void gradientMagnitude(const cv::Mat &image, float *M, float *O);

/* Compute gradient magnitude and orientation at each image location.
 * This code requires SSE2 to compile and run (most modern Intel and AMD
 * processors support SSE2). Please see: http://en.wikipedia.org/wiki/SSE2.
 * Image should be single (float datatype) values [0, 1]*/
void gradientMagnitude(const cv::Mat &image, cv::Mat &M, cv::Mat&O);

/* INPUTS
 * I          - [hxwxk] input k channel single image
 * OUTPUTS
 * vect<M>    - [(h/binSize)x(w/binSize)xM] M is the size of
 *                the vector and is equal to orientations * 3 + 5 */
void fhog(const cv::Mat &image, vector<Mat> &fhogs, int binSize, int orientations);
void fhog(const cv::Mat &image, Mat &fhogs, int binSize, int orientations);

/*******************************************************************************
 * Piotr's Computer Vision Matlab Toolbox      Version 3.30
 * Copyright 2014 Piotr Dollar & Ron Appel.  [pdollar-at-gmail.com]
 * Licensed under the Simplified BSD License [see external/bsd.txt]
 * Modified by Andres Solis Montero 2015.
 * Removed dependencies from mex matlab
 * and renamed functions to remove name collisions
 *******************************************************************************/

#include <iostream>
#include <math.h>
#include "string.h"

/*******************************************************************************/
// wrapper functions if compiling from C/C++
inline void  wrError(const char *errormsg) { throw errormsg; }
inline void* wrCalloc(size_t num, size_t size) { return calloc(num,size); }
inline void* wrMalloc(size_t size) { return malloc(size); }
inline void  wrFree(void * ptr) { free(ptr); }


// platform independent aligned memory allocation (see also alFree)
inline void* alMalloc( size_t size, int alignment ) {
    const size_t pSize = sizeof(void*), a = alignment-1;
    void *raw = wrMalloc(size + a + pSize);
    void *aligned = (void*) (((size_t) raw + pSize + a) & ~a);
    *(void**) ((size_t) aligned-pSize) = raw;
    return aligned;
}

// platform independent alignned memory de-allocation (see also alMalloc)
inline void alFree(void* aligned) {
    void* raw = *(void**)((char*)aligned-sizeof(void*));
    wrFree(raw);
}
/*******************************************************************************/
#include <emmintrin.h> // SSE2:<e*.h>, SSE3:<p*.h>, SSE4:<s*.h>
#define RETf inline __m128
#define RETi inline __m128i

// set, load and store values
RETf SET( const float &x ) { return _mm_set1_ps(x); }
RETf SET( float x, float y, float z, float w ) { return _mm_set_ps(x,y,z,w); }
RETi SET( const int &x ) { return _mm_set1_epi32(x); }
RETf LD( const float &x ) { return _mm_load_ps(&x); }
RETf LDu( const float &x ) { return _mm_loadu_ps(&x); }
RETf STR( float &x, const __m128 y ) { _mm_store_ps(&x,y); return y; }
RETf STR1( float &x, const __m128 y ) { _mm_store_ss(&x,y); return y; }
RETf STRu( float &x, const __m128 y ) { _mm_storeu_ps(&x,y); return y; }
RETf STR( float &x, const float y ) { return STR(x,SET(y)); }

// arithmetic operators
RETi ADD( const __m128i x, const __m128i y ) { return _mm_add_epi32(x,y); }
RETf ADD( const __m128 x, const __m128 y ) { return _mm_add_ps(x,y); }
RETf ADD( const __m128 x, const __m128 y, const __m128 z ) {
    return ADD(ADD(x,y),z); }
RETf ADD( const __m128 a, const __m128 b, const __m128 c, const __m128 &d ) {
    return ADD(ADD(ADD(a,b),c),d); }
RETf SUB( const __m128 x, const __m128 y ) { return _mm_sub_ps(x,y); }
RETf MUL( const __m128 x, const __m128 y ) { return _mm_mul_ps(x,y); }
RETf MUL( const __m128 x, const float y ) { return MUL(x,SET(y)); }
RETf MUL( const float x, const __m128 y ) { return MUL(SET(x),y); }
RETf INC( __m128 &x, const __m128 y ) { return x = ADD(x,y); }
RETf INC( float &x, const __m128 y ) { __m128 t=ADD(LD(x),y); return STR(x,t); }
RETf DEC( __m128 &x, const __m128 y ) { return x = SUB(x,y); }
RETf DEC( float &x, const __m128 y ) { __m128 t=SUB(LD(x),y); return STR(x,t); }
RETf MINsse( const __m128 x, const __m128 y ) { return _mm_min_ps(x,y); }
RETf RCP( const __m128 x ) { return _mm_rcp_ps(x); }
RETf RCPSQRT( const __m128 x ) { return _mm_rsqrt_ps(x); }

// logical operators
RETf AND( const __m128 x, const __m128 y ) { return _mm_and_ps(x,y); }
RETi AND( const __m128i x, const __m128i y ) { return _mm_and_si128(x,y); }
RETf ANDNOT( const __m128 x, const __m128 y ) { return _mm_andnot_ps(x,y); }
RETf OR( const __m128 x, const __m128 y ) { return _mm_or_ps(x,y); }
RETf XOR( const __m128 x, const __m128 y ) { return _mm_xor_ps(x,y); }

// comparison operators
RETf CMPGT( const __m128 x, const __m128 y ) { return _mm_cmpgt_ps(x,y); }
RETf CMPLT( const __m128 x, const __m128 y ) { return _mm_cmplt_ps(x,y); }
RETi CMPGT( const __m128i x, const __m128i y ) { return _mm_cmpgt_epi32(x,y); }
RETi CMPLT( const __m128i x, const __m128i y ) { return _mm_cmplt_epi32(x,y); }

// conversion operators
RETf CVT( const __m128i x ) { return _mm_cvtepi32_ps(x); }
RETi CVT( const __m128 x ) { return _mm_cvttps_epi32(x); }

#undef RETf
#undef RETi
/*******************************************************************************/
// compute x and y gradients for just one column (uses sse)
void grad1( float *I, float *Gx, float *Gy, int h, int w, int x );

// compute x and y gradients at each location (uses sse)
void grad2( float *I, float *Gx, float *Gy, int h, int w, int d ) ;

// build lookup table a[] s.t. a[x*n]~=acos(x) for x in [-1,1]
float* acosTable() ;

// compute gradient magnitude and orientation at each location (uses sse)
void gradMag( float *I, float *M, float *O, int h, int w, int d, bool full );

// normalize gradient magnitude at each location (uses sse)
void gradMagNorm( float *M, float *S, int h, int w, float norm );

// helper for gradHist, quantize O and M into O0, O1 and M0, M1 (uses sse)
void gradQuantize( float *O, float *M, int *O0, int *O1, float *M0, float *M1,
                  int nb, int n, float norm, int nOrients, bool full, bool interpolate );

// compute nOrients gradient histograms per bin x bin block of pixels
void gradHist( float *M, float *O, float *H, int h, int w,
              int bin, int nOrients, int softBin, bool full );

/******************************************************************************/

// HOG helper: compute 2x2 block normalization values (padded by 1 pixel)
float* hogNormMatrix( float *H, int nOrients, int hb, int wb, int bin );

// HOG helper: compute HOG or FHOG channels
void hogChannels( float *H, const float *R, const float *N,
                 int hb, int wb, int nOrients, float clip, int type );

// compute HOG features
void hog( float *M, float *O, float *H, int h, int w, int binSize,
         int nOrients, int softBin, bool full, float clip );

// compute FHOG features
void fhog( float *M, float *O, float *H, int h, int w, int binSize,
          int nOrients, int softBin, float clip );



template <typename _Tp> static
void olbp(InputArray _src, OutputArray _dst) {
    // get matrices
    Mat src = _src.getMat();
    // allocate memory for result
    //_dst.create(src.rows-2, src.cols-2, CV_8UC1);
    _dst.create(src.rows, src.cols, CV_32FC1);
    Mat dst = _dst.getMat();
    // zero the result matrix
    dst.setTo(0);
    // calculate patterns
    for(int i=1;i<src.rows-1;i++) {
        for(int j=1;j<src.cols-1;j++) {
            _Tp center = src.at<_Tp>(i,j);
            unsigned char code = 0;
            code |= (src.at<_Tp>(i-1,j-1) >= center) << 7;
            code |= (src.at<_Tp>(i-1,j) >= center)   << 6;
            code |= (src.at<_Tp>(i-1,j+1) >= center) << 5;
            code |= (src.at<_Tp>(i,j+1) >= center)   << 4;
            code |= (src.at<_Tp>(i+1,j+1) >= center) << 3;
            code |= (src.at<_Tp>(i+1,j) >= center)   << 2;
            code |= (src.at<_Tp>(i+1,j-1) >= center) << 1;
            code |= (src.at<_Tp>(i,j-1) >= center)   << 0;
            int _c = code;
            dst.at<float>(i-1,j-1) = (float)_c/255.0;
        }
    }
}


// Fast OLBP using grayscale images only
template <typename _Tp> static
void folbp(InputArray _src, OutputArray _dst) {
    // get matrices
    Mat src = _src.getMat();
    
    // allocate memory for result
    _dst.create(src.rows,
                src.cols,
                CV_32FC1);
    
    Mat dst = _dst.getMat();
    // zero the result matrix
    
    dst.setTo(0);
    // calculate patterns
    for(int i = 1;i < src.rows - 1; i++)
    {
        _Tp *top = src.ptr<_Tp>(i - 1);
        _Tp *cen = src.ptr<_Tp>(i);
        _Tp *bot = src.ptr<_Tp>(i + 1);
        float *dcen = dst.ptr<float>(i-1);
        for(int j = 1;j < src.cols - 1; j++)
        {
            _Tp center = cen[j];
            unsigned char code = 0;
            int prev = j-1, mid = j, next = j+1;
            code |= (top[prev] >= center) << 7;
            code |= (top[mid]  >= center) << 6;
            code |= (top[next] >= center) << 5;
            code |= (cen[next] >= center) << 4;
            code |= (bot[next] >= center) << 3;
            code |= (bot[mid]  >= center) << 2;
            code |= (bot[prev] >= center) << 1;
            code |= (cen[prev] >= center) << 0;
            int _c = code;
            dcen[prev] = (float)_c/255.0;
        }
    }
}


#endif