/*******************************************************
 *  tracker_factory.cpp (VivaTracker)
 *  <asolis@hotmail.ca>
 *   PhD Student.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 * Project can not be copied and/or distributed without the express
 * permission of Andrés Solís Montero
 * Created by Andrés Solís Montero on 2015-02-17.
 *******************************************************/

#include "factories.h"


Ptr<Tracker> TrackerFactory::createTracker(TRACKING_METHOD method)
{
    Ptr<Tracker> tracker;
    
    switch (method)
    {
//        case (TRACKING_METHOD::OPENTLD):
//            tracker = new OpenTLD();
//            break;
//        case (TRACKING_METHOD::CMT):
//            tracker = new CMTtracker();
//            break;
//        case (TRACKING_METHOD::CSK):
//            tracker = new CSKTracker();
//            break;
//        case (TRACKING_METHOD::ALIEN):
//            tracker = new AlienTracker();
//            break;
//        case (TRACKING_METHOD::STRUCK):
//            tracker = new STRUCKtracker();
//            break;
            
        case (TRACKING_METHOD::KCF_P_G):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::GRAY);
            break;
        case (TRACKING_METHOD::KCF_P_RGB):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::RGB);
            break;
        case (TRACKING_METHOD::KCF_P_FHOG):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::FHOG);
            break;
        case (TRACKING_METHOD::KCF_P_HSV):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::HSV);
            break;
        case (TRACKING_METHOD::KCF_P_HLS):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::HLS);
            break;
        
        case (TRACKING_METHOD::KCF_G_G):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::GRAY);
            break;
        case (TRACKING_METHOD::KCF_G_RGB):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::RGB);
            break;
        case (TRACKING_METHOD::KCF_G_FHOG):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::FHOG);
            break;
        case (TRACKING_METHOD::KCF_G_HSV):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::HSV);
            break;
        case (TRACKING_METHOD::KCF_G_HLS):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::HLS);
            break;
            
        case (TRACKING_METHOD::DCF_G):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::GRAY);
            break;
        case (TRACKING_METHOD::DCF_RGB):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::RGB);
            break;
        case (TRACKING_METHOD::DCF_FHOG):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::FHOG);
            break;
        case (TRACKING_METHOD::DCF_HSV):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::HSV);
            break;
        case (TRACKING_METHOD::DCF_HLS):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::HLS);
            break;
            
        case (TRACKING_METHOD::KCF_P_G_S):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::GRAY, true);
            break;
        case (TRACKING_METHOD::KCF_P_RGB_S):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::RGB, true);
            break;
        case (TRACKING_METHOD::KCF_P_FHOG_S):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::FHOG, true);
            break;
        case (TRACKING_METHOD::KCF_P_HSV_S):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::HSV, true);
            break;
        case (TRACKING_METHOD::KCF_P_HLS_S):
            tracker = new SKCFDCF(KType::POLYNOMIAL, KFeat::HLS, true);
            break;
            
        case (TRACKING_METHOD::KCF_G_G_S):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::GRAY, true);
            break;
        case (TRACKING_METHOD::KCF_G_RGB_S):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::RGB, true);
            break;
        case (TRACKING_METHOD::KCF_G_FHOG_S):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::FHOG, true);
            break;
        case (TRACKING_METHOD::KCF_G_HSV_S):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::HSV, true);
            break;
        case (TRACKING_METHOD::KCF_G_HLS_S):
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::HLS, true);
            break;
            
        case (TRACKING_METHOD::DCF_G_S):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::GRAY, true);
            break;
        case (TRACKING_METHOD::DCF_RGB_S):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::RGB, true);
            break;
        case (TRACKING_METHOD::DCF_FHOG_S):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::FHOG, true);
            break;
        case (TRACKING_METHOD::DCF_HSV_S):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::HSV, true);
            break;
        case (TRACKING_METHOD::DCF_HLS_S):
            tracker = new SKCFDCF(KType::LINEAR, KFeat::HLS, true);
            break;
            
            
        default:
            tracker = new SKCFDCF(KType::GAUSSIAN, KFeat::FHOG, true);
            break;
    }
    return tracker;
}
