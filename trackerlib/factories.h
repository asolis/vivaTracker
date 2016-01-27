/*******************************************************
 *  tracker_factory.h (VivaTracker)
 *  <asolis@hotmail.ca>
 *   PhD Student.
 *   SITE, University of Ottawa
 *   800 King Edward Ave.
 *   Ottawa, On., K1N 6N5, Canada.
 * Project can not be copied and/or distributed without the express
 * permission of Andrés Solís Montero
 * Created by Andrés Solís Montero on 2015-02-17.
 *******************************************************/

#ifndef __VivaTracker__factory__
#define __VivaTracker__factory__

#include <iostream>

#include "tracker.h"
#include "tracking_process.h"
//#include "CMTtracker.h"
//#include "opentld.h"
//#include "STRUCKtracker.h"
//#include "AlienTracker.h"
//#include "csk_tracker.h"
#include "skcfdcf.h"



enum class TRACKING_METHOD{OPENTLD, CSK, CMT, STRUCK, ALIEN, AREA,
        KCF_P_G, KCF_P_RGB, KCF_P_FHOG, KCF_P_HSV, KCF_P_HLS,
        KCF_G_G, KCF_G_RGB, KCF_G_FHOG, KCF_G_HSV, KCF_G_HLS,
        DCF_G, DCF_RGB, DCF_FHOG, DCF_HSV, DCF_HLS,
        KCF_P_G_S, KCF_P_RGB_S, KCF_P_FHOG_S, KCF_P_HSV_S, KCF_P_HLS_S,
        KCF_G_G_S, KCF_G_RGB_S, KCF_G_FHOG_S, KCF_G_HSV_S, KCF_G_HLS_S,
        DCF_G_S, DCF_RGB_S, DCF_FHOG_S, DCF_HSV_S,DCF_HLS_S,
};

class TrackerFactory
{
public:
    static Ptr<Tracker> createTracker(TRACKING_METHOD method = TRACKING_METHOD::AREA);
    static Ptr<Tracker> createTracker(KType type, KFeat feat, bool scale = false)
    {
        Ptr<Tracker> tracker = new SKCFDCF(type, feat, scale);
        return tracker;
    }
};


#endif /* defined(__VivaTracker__tracker_factory__) */
