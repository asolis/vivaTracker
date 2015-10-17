# cmake vivaVideo
# set minimum required version for cmake
cmake_minimum_required(VERSION 2.8)

# define the project name
set(project_name "vivaVideo")

# set the project namee
project("${project_name}")

# display the project name
message("Project name: ${project_name}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive")

# add opencv package to the project
find_package( OpenCV REQUIRED )
# add opencv include directories to the project
include_directories( ${OpenCV_INCLUDE_DIRS} ) 

set(OpenCV_LIBRARIES "opencv_video;opencv_ts;opencv_imgproc;opencv_highgui;opencv_features2d;opencv_core;")


# add sub-directories
subdirs(viva)
include_directories (${vivaVideo_SOURCE_DIR}/viva) 
# add executable
add_executable(${project_name} main.cpp)


# link libraries
target_link_libraries( ${project_name} ${OpenCV_LIBRARIES})
target_link_libraries( ${project_name}  vivalib )
