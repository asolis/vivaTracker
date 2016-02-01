#ifndef OPENTLD_H
#define OPENTLD_H

#include <viva.h>
#include <TLD.h>
#include <tracker.h>

using namespace viva;
using namespace std;
using namespace cv;
using namespace tld;

class OpenTLD: public Tracker
{
    
    Ptr<TLD> tld;
    
public:
    
    OpenTLD():tld()
    {
        
    }
    
    /*
     * This represent the current tracked area in clockwise order. e.g. :
     * topLeft - > topRight -> bottomRight -> bottomLeft.
     * @param vector<Point2f> &pts. The tracked area will be filled  in this vector
     */
    void getTrackedArea(vector<Point2f> &pts)
    {
        if (tld->currBB)
        {
            pts.push_back(tld->currBB->tl());
            pts.push_back(Point2f(tld->currBB->tl().x, tld->currBB->br().y));
            pts.push_back(tld->currBB->br());
            pts.push_back(Point2f(tld->currBB->br().x, tld->currBB->tl().y));
        }
        else
        {
            pts.push_back(Point2f(-1,-1));
            pts.push_back(Point2f(-1,-1));
            pts.push_back(Point2f(-1,-1));
            pts.push_back(Point2f(-1,-1));
        }
    }
   
    /*
     * Initialize the tracker using the first imaga with the selected area
     * using two points. The two points create a rectangular selection inside the image.
     * @param Mat &image,       The first reference frame where the area is specified.
     * @param Rect &rect  Rectangular region.
     */
    void initialize(const cv::Mat &image,
                            const cv::Rect &rect)
    {
        Mat gray;
        toGray(image, gray);
        
        if (tld)
            tld->release();
        
        tld = new TLD();
        tld->detectorCascade->imgWidth = gray.cols;
        tld->detectorCascade->imgHeight= gray.rows;
        tld->detectorCascade->imgWidthStep = gray.step;
        tld->trackerEnabled = true;
        tld->alternating = false;
        tld->learningEnabled = true;
        tld->detectorCascade->varianceFilter->enabled = true;
        tld->detectorCascade->ensembleClassifier->enabled = true;
        tld->detectorCascade->nnClassifier->enabled = true;
        tld->detectorCascade->useShift = true;
        tld->detectorCascade->shift = 0.1;
        tld->detectorCascade->minScale = -10;
        tld->detectorCascade->maxScale = 10;
        tld->detectorCascade->minSize = 25;
        tld->detectorCascade->numTrees = 10;
        tld->detectorCascade->numFeatures = 10;
        tld->detectorCascade->nnClassifier->thetaTP = 0.65;
        tld->detectorCascade->nnClassifier->thetaFP = 0.5;
        srand(0);
        Rect tmp = rect;
        tld->selectObject(gray, &tmp);
    };
    /*
     * This should be called every time after the tracker is initialized.
     * The image is the current frame being processed.
     * @param Mat &image. processing frame.
     */
    void processFrame(const cv::Mat &image)
    {
        Mat gray;
        toGray(image, gray);
        tld->processImage(gray);
    };
    
    
    static void toGray(const Mat &input, Mat &output)
    {
        if (input.channels() == 3)
            cvtColor(input, output, CV_BGR2GRAY);
        else
            input.copyTo(output);
    }
    
    /*
     * Each tracker has an string description of its name
     * or condition.
     * e.g.  "OpenTLD" , "CMT", etc.
     */
    string virtual getDescription()
    {
        return "opentld";
    };
};

#endif
