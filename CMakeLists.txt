CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(PROJECT_NAME "vivaTracker")
PROJECT("${PROJECT_NAME}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# add opencv package to the project
FIND_PACKAGE( OpenCV REQUIRED )
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} ) 

MESSAGE(STATUS "Project Name: ${PROJECT_NAME}")
MESSAGE(STATUS "    OpenCV version: ${OpenCV_VERSION} ")
MESSAGE(STATUS "    libraries: ${OpenCV_LIBS}")
MESSAGE(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

FILE(GLOB files
	"*.h"
	"*.cpp"
)
ADD_EXECUTABLE(${PROJECT_NAME} ${files})

# Include libraries and trackers to project
SUBDIRS(vivalib trackerlib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/vivalib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/trackerlib)
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${OpenCV_LIBS} vivalib trackerlib)

#Needed because Struck tracker
SET(EIGEN3_INCLUDE_DIRS "" CACHE FILEPATH "Path to eigen3 directory")
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIRS})

#Datasets and Trackers
INCLUDE("macros.txt")
CHECK_DATASETS()
CHECK_TRACKERS()

# Copy resources in source folder to build folders
FILE(GLOB hidden
	".*"
)
FILE(GLOB resources
  "*.*"
)
LIST(REMOVE_ITEM resources ${files} ${hidden} "${CMAKE_SOURCE_DIR}/CMakeLists.txt" "${CMAKE_SOURCE_DIR}/macros.txt")
FILE(COPY ${resources} DESTINATION "Debug")
FILE(COPY ${resources} DESTINATION "Release")