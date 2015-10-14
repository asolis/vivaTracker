# cmake CoreLib
include_directories(${videoCapture_SOURCE_DIR}/core)

file(GLOB Core_Files
	"*.h"
	"*.cpp"
)

add_library(CoreLib  ${Core_Files})

