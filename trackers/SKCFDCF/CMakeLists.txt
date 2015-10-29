# cmake skcflib
include_directories(${VivaTracker_SOURCE_DIR}/viva)
include_directories(${VivaTracker_SOURCE_DIR}/trackers)

file(GLOB AT_Files
	"*.h"
	"*.cpp"
)

add_library(skcflib ${AT_Files})

