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
#ifndef __trackers__ktrackers__
#define __trackers__ktrackers__

#include <vector>
#include <functional>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "gradient.h"

using namespace cv;
using namespace std;



// Gaussian & Polynomial == KCF
// Linear = DCF
enum class KType{GAUSSIAN, POLYNOMIAL, LINEAR};
enum class KFeat{GRAY, RGB, FHOG, HLS, HSV};

struct ConfigParams{
    float padding = 1.5;  //Extra area surrounding the target
    float lambda  = 1e-4; //Regularization
    float output_sigma_factor = 0.1; //Spatial bandwith (proportional to target)
    
    KFeat kernel_feature = KFeat::GRAY;    // Gray, RGB, or HOG
    KType kernel_type    = KType::GAUSSIAN;// Gaussian or Polynomial (KCF), Linear (DCF)
    float kernel_sigma  = 0.2; //gaussian kernel bandwidth
    int   kernel_poly_a = 1;   //polynomial kernel additive term
    int   kernel_poly_b = 7;   //polynomial kernel exponent
    
    float interp_factor = 0.075;//linear interpolation factor for adaptation
    int   hog_orientations = 1;
    int   cell_size = 1;
    bool  scale     = false;     //Toggle for scale computation
    
    // 0 value uses compact CCS packed format for the spectrum. DFT_COMPLEX_OUTPUT;
    //Look for OpenCV dft function flags parameter
    int flags   = 0;
    
    
    ConfigParams(KType ktype, bool compScale):
    padding(1.5), lambda(1e-4), output_sigma_factor(0.1),
    kernel_feature(KFeat::GRAY), kernel_type(ktype), kernel_sigma(0.2),
    kernel_poly_a(1), kernel_poly_b(7), interp_factor(0.075), hog_orientations(1),
    cell_size(1),scale(compScale), flags(0)
    {}
};

/* Default Configuration Parameters for Grayscale kernel features */
struct GrayConfigParams: public ConfigParams {
    GrayConfigParams(KType kernel_t, bool scale): ConfigParams(kernel_t, scale)
    {
        kernel_feature = KFeat::GRAY;
        interp_factor  = 0.075;
        kernel_sigma   = 0.2;
        kernel_poly_a  = 1;
        kernel_poly_b  = 7;
        cell_size      = 1;
    }
};

struct HLSConfigParams: public ConfigParams {
    HLSConfigParams(KType kernel_t, bool scale): ConfigParams(kernel_t, scale)
    {
        kernel_feature = KFeat::HLS;
        interp_factor  = 0.075;
        kernel_sigma   = 0.2;
        kernel_poly_a  = 1;
        kernel_poly_b  = 7;
        cell_size      = 1;
    }
};

struct HSVConfigParams: public ConfigParams {
    HSVConfigParams(KType kernel_t, bool scale): ConfigParams(kernel_t, scale)
    {
        kernel_feature = KFeat::HSV;
        interp_factor  = 0.075;
        kernel_sigma   = 0.2;
        kernel_poly_a  = 1;
        kernel_poly_b  = 7;
        cell_size      = 1;
    }
};


/* Default Configuration Parameters for RGB kernel features */
struct RGBConfigParams: public ConfigParams {
    RGBConfigParams(KType kernel_t, bool scale): ConfigParams(kernel_t, scale)
    {
        kernel_feature = KFeat::RGB;
        interp_factor  = 0.075;
        kernel_sigma   = 0.2;
        kernel_poly_a  = 1;
        kernel_poly_b  = 7;
        cell_size      = 1;
    }
};

/* Default Configuration Parameters for HOG kernel features */
struct FHOGConfigParams: public ConfigParams {
    FHOGConfigParams(KType kernel_t, bool scale): ConfigParams(kernel_t, scale)
    {
        kernel_feature   = KFeat::FHOG;
        interp_factor    = 0.02;
        kernel_sigma     = 0.5;
        kernel_poly_a    = 1;
        kernel_poly_b    = 9;
        hog_orientations = 9;
        cell_size        = 4;
    }
};

/* Internal representation of the object by size and location */
struct TObj{
    bool       initiated = false;
    Size2i    windowSize;   // Optimal window size for fft performance
    Size2d          size;   // Current size of the object
    Point2f       center;   // Center location of the object
    vector<Mat> model_xf;   // Fourier Domain: model of the tracking obj.
    Mat     model_alphaf;   // Fourier Domain: Kernel Ridge Regression.
};

struct KFlowConfigParams
{
    int winsize_ncc = 10;    //SIZE OF THE WINDOWS FOR NCC COMPUTATION
    int win_size_lk = 15;    //SIZE OF THE WINDOWS FOR LUKAS KANADE
    int level       = 0;     //PYRAMID LEVEL OF LK
    int medFBThreshold = 10; // Threshold for the back-projection
    Size winLK = Size(win_size_lk, win_size_lk);
    TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 20, 0.03);
    int method = CV_TM_CCORR_NORMED;
    int transMode = 1;     //O : Median.  1: Centroid
    int ptsThreshold = 5;
    
    //Shi-Tomasi features  / Harris Corner Detector
    double     qualityLevel = 0.01;
    double     minDistance  = 3;
    int           blockSize = 3;
    bool  useHarrisDetector = false;
    double                k = 0.04;
    int maxCorners          = 100;
    
};

class KFlow
{
    KFlowConfigParams _params;
public:
    vector<Point2f> _pts;
    vector<float> _weights;
    Mat _curr;
    double _scale;
    
    
    KFlow():_params(), _pts(), _curr(), _scale(1.)
    {}
    
    static void toGray(const Mat &input, Mat &output)
    {
        if (input.channels() == 3)
            cvtColor(input, output, CV_BGR2GRAY);
        else
            input.copyTo(output);
    }
    static void toBGR(const Mat &input, Mat &output)
    {
        if (input.channels() == 1)
            cvtColor(input, output, CV_GRAY2BGR);
        else
            input.copyTo(output);
    }
    
    double getScale()
    {
        return _scale;
    }
    
    
    void extractPoints(const Mat &frame,
                       const Size2d size)
    {
        toGray(frame, _curr);
        extractPoints(_curr, _params, size, _pts, _weights);
    }
    void processFrame(const Mat &frame,
                      const Mat &weights,
                      const Size2d &size,
                      const Point2f &shift)
    {
        Mat tmp;
        toGray(frame, tmp);
        
        _scale = 1.0;
        if (_pts.size() > 0)
        {
            vector<Point2f> to;
            flowForwardBackward(_curr, tmp, _pts, to, _params);
            _scale = transform(_pts, to, _weights, _params);
            
            int inliers = 0, outliers = 0;
            for (size_t i = 0; i < to.size(); ++i)
            {
                Point2f _tmp(_pts[i].x - (to[i].x + shift.x * _scale),
                             _pts[i].y - (to[i].y + shift.y * _scale));
                
                if (norm(_tmp) < _params.ptsThreshold)
                    inliers++;
                else
                    outliers++;
            }
            
            if (outliers > inliers)
                _scale = 1.0;
                
        }
        
        tmp.copyTo(_curr);
    }
    
public:
    static void extractPoints(const Mat& patch,
                              const KFlowConfigParams &p,
                              const Size2d  &size,
                              vector<Point2f> &points,
                              vector<float> &weights)
    {
        
        assert(patch.type() == CV_8UC1);
        
        int width = patch.cols;
        int height= patch.rows;
        float *w = new float[width];
        float *h = new float[height];
        for (size_t i = 0; i < width; ++i )
            w[i] = .5 * ( 1. - cos((2.* CV_PI* i)/(width - 1)));
        for (size_t i = 0; i < height; ++i)
            h[i] = .5 * ( 1. - cos((2.* CV_PI* i)/(height- 1)));
        
        int POINTS = 100;
        double wTHRESHOLD = 0.85;
        
        RNG rng(0xFFFFFFFF);
        Point tl(max(0.0,patch.cols/2.0 - floor(size.width/2.0)),
                 max(0.0,patch.rows/2.0 - floor(size.height/2.0)));
        
        Point br(tl + Point(floor(size.width), floor(size.height)));
        
        Mat mask = Mat::zeros(patch.size(),CV_8UC1);
        rectangle(mask, tl, br, Scalar(255), CV_FILLED);
        vector<Point2f> _tmp;
        goodFeaturesToTrack(patch,
                            _tmp,
                            p.maxCorners,
                            p.qualityLevel,
                            p.minDistance,
                            mask,
                            p.blockSize,
                            p.useHarrisDetector,
                            p.k);
        weights.clear();
        points.clear();
        
        for (int i = 0; i < _tmp.size(); i++)
        {
            int _x_ = floor(_tmp[i].x);
            int _y_ = floor(_tmp[i].y);
            double _weight =w[_x_] * h[_y_];
            
            if (_weight < wTHRESHOLD) continue;
            weights.push_back(_weight);
            points.push_back(_tmp[i]);
                
        }

        for (int i = 0; i < POINTS ; ++i)
        {
            double nX = rng.uniform((double)tl.x,(double) (br.x));
            double nY = rng.uniform((double)tl.y,(double) (br.y));
            int _x_ = floor(nX);
            int _y_ = floor(nY);
            double _weight =w[_x_] * h[_y_];
            if (_weight <  wTHRESHOLD) continue;
        
            weights.push_back(_weight);
            points.push_back(Point2f(nX,nY));
        
        }
        
        delete[] w;
        delete[] h;
            
    }
    
    
    
    /*
     * tracks area B to BNew using two images frame I and J.
     */
    static void flowForward(const Mat &I,
                            const Mat &J,
                            vector<Point2f> &from,
                            vector<Point2f> &to,
                            const KFlowConfigParams &p);
    
    static void flowForwardBackward(const Mat &I,
                                    const Mat &J,
                                    vector<Point2f> &from,
                                    vector<Point2f> &to,
                                    const KFlowConfigParams &p);
    
    
    /*
     *  Transform rectangular region B into BNew using the matching points
     *  from start to tracked.
     */
    static void transform(Rect_<float> &B,
                          Rect_<float> &BNew,
                          const vector<Point2f> &start,
                          const vector<Point2f> &tracked,
                          const KFlowConfigParams &p);
    
    /*
     *  Returns the scale and the translation between the two sets of points.
     */
    static double transform(const vector<Point2f> &start,
                            const vector<Point2f> &tracked,
                            Point2f &shift,
                            const KFlowConfigParams &p);
    
    /*
     *  Returns the scale between the two sets of points.
     */
//    static double transform(const vector<Point2f> &start,
//                            const vector<Point2f> &tracked,
//                            const KFlowConfigParams &p);
    static double transform(const vector<Point2f> &start,
                            const vector<Point2f> &tracked,
                            const vector<float> &weights,
                            const KFlowConfigParams &p);
    
    
    //    static void flow(const Mat &prev,
    //              const Mat &image,
    //              vector<Point2f> &from,
    //              vector<Point2f> &to);
    
private:
    /*
     * Computes the NCC value for points from one frame to the other
     */
    static void NCC(const Mat &I,
                    const Mat &J,
                    vector<Point2f> &ptsI,
                    vector<Point2f> &ptsJ,
                    vector<uchar> &status,
                    vector<float> &result,
                    const KFlowConfigParams &p);
    
    /*
     * Computes Euclidean distance (NORM2) between two list of points.
     */
    static void NORM2(vector<Point2f> &ptsI,
                      vector<Point2f> &ptsJ,
                      vector<float> &distances);
    
    /**
     * Returns median of the array. Changes array!
     * @param arr the array
     * @pram n length of array
     */
    static float getMedian(float arr[], int n);
    
    /**
     * Calculates Median of the array. Don't change array(makes copy).
     * @param arr the array
     * @pram n length of array
     */
    static float getMedianUnmanaged(float arr[], int n);
};


class KTrackers
{
    
public:
    KTrackers(KType type, KFeat feat, bool scale);
    
    void setArea(const RotatedRect &rect);
    void getTrackedArea(vector<Point2f> &pts);
    void processFrame(const cv::Mat &frame);
    
    void getTrackedPoints(vector<Point2f> &pts)
    {
        pts.clear();
        for (size_t i = 0; i < _flow._pts.size(); ++i)
        {
            pts.push_back(_flow._pts[i] + _ptl);
        }
    }
    int getNumberOfTrackedPoints()
    {
        return _flow._pts.size();
    }
    
    KType getType()
    {
        return _params.kernel_type;
    }
    KFeat getFeature()
    {
        return _params.kernel_feature;
    }
    bool  hasScale()
    {
        return _params.scale;
    }
    
protected:
    TObj         _target;
    ConfigParams _params;
    KFlow        _flow;
    
    Point2f      _ptl;
    
    
private:
    template<typename T>
    static void gaussianResponse(Mat &input, double _a, double _b, double _c)
    {
        T* data = (T*)input.data;
        for (size_t i = 0; i < input.rows * input.cols; ++i)
        {
            T &pxl = data[i];
            pxl = saturate_cast<T>(exp(_a * max(0., (_b - 2 * pxl)/_c )));
        }
    }
    
    template<typename T>
    static void polynomialResponse(Mat &input,double _N, double _a, double _b)
    {
        T* data = (T*)input.data;
        for (size_t i = 0; i < input.rows * input.cols; ++i)
        {
            T &pxl = data[i];
            pxl = saturate_cast<T>(pow(pxl/_N + _a, _b));
        }
    }
    
    
    static void getFeatures(const Mat& patch,
                            const ConfigParams &params,
                            const Mat& windowFunction,
                            vector<Mat> &features);
    
    static void getPoints(const Mat& image,
                          const Mat& patch,
                          const ConfigParams &params,
                          const TObj &obj,
                          vector<Point2f> &points,
                          Point2f &tl);
    
    //  Computes the dft of each channel using the ConfigParams.flag value
    //  A value of 0 will compute the dft using the CCS packed format. Each Mat will be of type CV_32FC1
    //  DFT_COMPLEX_VALUE will compute the whole spectrum with type CV_32FC2.
    static void fft2(vector<Mat> &fft2, const ConfigParams &params);//inplace
    static void fft2(const vector<Mat> &features,vector<Mat> &fft2, const ConfigParams &params);
    static void fft2(Mat &fft2, const ConfigParams &params); //inplace
    static void fft2(const Mat &features, Mat &fft2, const ConfigParams &params);
    
    //   GAUSSIAN_CORRELATION Gaussian Kernel at all shifts, i.e. kernel correlation.
    //   Evaluates a Gaussian kernel with bandwidth SIGMA for all relative
    //   shifts between vector input images X and Y, which each element must be MxN. The result
    //   is an MxN map of responses.
    //   The vectors contains the channels of the input images in the frequency domain (dft).
    //   Each channel is assumed to be one of CV_32FC1 || CV_32FC2 || CV_64FC1 || CV_64FC2.
    //   Single channel inputs (CV_32FC1 || CV_64FC1) should be packed in format CCS (complex-conjugate-symmetrical).
    //   The format was borrowed from IPL (Intel* Image Processing Library). It should boost the performance 30~40%;
    //   INPUTS:
    //        XF  vector of channels in frequency domain.
    //        YF  vector of channels in frequency domain.
    //   OUTPUT:
    //        kf  in frequency domain
    static void gaussian_correlation(const vector<Mat> &xf,
                                     const vector<Mat> &yf,
                                     const ConfigParams &params,
                                     Mat &kf,
                                     bool autocorrelation = false);

   
    
    
    //   LINEAR_CORRELATION Linear Kernel at all shifts, i.e. correlation.
    //   Computes the dot-product for all relative shifts between vector input images
    //   X and Y, which each element must be MxN. The result is an MxN map of
    //   responses.
    //   The vectors contains the channels of the input images in the frequency domain (dft).
    //   Each channel is assumed to be one of CV_32FC1 || CV_32FC2 || CV_64FC1 || CV_64FC2.
    //   Single channel inputs (CV_32FC1 || CV_64FC1) should be in packed format CCS (complex-conjugate-symmetrical).
    //   The format was borrowed from IPL (Intel* Image Processing Library). It should boost the performance 30~40%;
    //   INPUTS:
    //        XF  vector of channels in frequency domain.
    //        YF  vector of channels in frequency domain.
    //   OUTPUT:
    //        kf  in frequency domain
    static void linear_correlation(const vector<Mat> &xf,
                                   const vector<Mat> &yf,
                                   Mat &kf);

    
    //   POLYNOMIAL_CORRELATION Polynomial Kernel at all shifts, i.e. kernel correlation.
    //   Evaluates a polynomial kernel with constant A and exponent B, for all
    //   relative shifts between vector input images XF and YF, which each element must be MxN.
    //   The result is an MxN map of responses.
    //   The vectors contains the channels of the input images in the frequency domain (dft).
    //   Each channel is assumed to be one of CV_32FC1 || CV_32FC2 || CV_64FC1 || CV_64FC2.
    //   Single channel inputs (CV_32FC1 || CV_64FC1) should be in packed format CCS (complex-conjugate-symmetrical).
    //   The format was borrowed from IPL (Intel* Image Processing Library). It should boost the performance 30~40%;
    //   INPUTS:
    //        XF  vector of channels in frequency domain.
    //        YF  vector of channels in frequency domain.
    //   OUTPUT:
    //        kf  in frequency domain
    static void polynomial_correlation(const vector<Mat> &xf,
                                       const vector<Mat> &yf,
                                       const ConfigParams &params,
                                       Mat &kf);

    // Equation for fast detection
    // location is at the maximum response. we must take into
    // account the fact that, if the target doesn't move, the peak
    // will appear at the top-left corner, not at the center,
    // the responses wrap around cyclically.
    static double fastDetection(const Mat &modelAlphaF,
                                const Mat &kzf,
                                Point &location);
    
    
    static void  getPatch(const Mat& image,
                          const Point2f &loc,
                          const Size &sz,
                          Mat &output);
    
    // GAUSSIAN_SHAPED_LABELS
    //    Gaussian-shaped labels for all shifts of a sample.
    // 
    //    LABELS = GAUSSIAN_SHAPED_LABELS(SIGMA, SZ)
    //    Creates an array of labels (regression targets) for all shifts of a
    //    sample of dimensions SZ. The output will have size SZ, representing
    //    one label for each possible shift. The labels will be Gaussian-shaped,
    //    with the peak at 0-shift (top-left element of the array), decaying
    //    as the distance increases, and wrapping around at the borders.
    //    The Gaussian function has spatial bandwidth SIGMA.
    static void gaussian_shaped_labels(float sigma, const Size &sz, Mat &labels);
    static void gaussian_shaped_labels(float sigmaW, float sigmaH, const Size &sz, Mat &labels);
    
    //  Filtering window
    static void  gaussianWindow(const Size &sz,
                                float sigmaW,
                                float sigmaH,
                                Mat &filter);
    
    //  Filtering window
    static void  hannWindow(const Size &sz, Mat &filter);
    
    //  Equation for fast trainning
    static void fastTraining(const Mat &yf,
                             const Mat &kf,
                             const ConfigParams& params,
                             Mat &alphaf);
    //  Learning the modle
    static void learn(vector<Mat> &modelXf, const vector<Mat> &xf,
                      Mat         &modelAlphaF, const Mat &alphaf,
                      const ConfigParams& params);
    //  divides two spectrum signals, in Complex and CCS format.
    //  The parameters lambda is just a regularization term to avoid division by zero.
    static void divSpectrums( InputArray _srcA, InputArray _srcB,
                             OutputArray _dst, int flags, bool conjB = false ,double lambda = 1e-4);
    //  Sum all the real values of the spectrum. 
    static double sumSpectrum(const Mat &mat, const ConfigParams &params);
};




#endif
