# cmake vivalib
include_directories(${videoVideo_SOURCE_DIR}/viva)

file(GLOB viva_files
	"*.h"
	"*.cpp"
)

add_library(vivalib  ${viva_files})

