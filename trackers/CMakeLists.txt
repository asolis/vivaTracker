# cmake trackerlib
include_directories(${vivaTracker_SOURCE_DIR}/viva)
include_directories(${vivaTracker_SOURCE_DIR}/trackers)
subdirs(SKCFDCF)

file(GLOB Tracker_Files
	"*.h"
	"*.cpp"
)

add_library(trackerlib ${Tracker_Files})

