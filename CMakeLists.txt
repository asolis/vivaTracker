CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
INCLUDE("macros.txt")

SET(PROJECT_NAME "vivaTracker")
PROJECT("${PROJECT_NAME}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# add opencv package to the project
FIND_PACKAGE( OpenCV REQUIRED )
INCLUDE_DIRECTORIES( "/usr/local/Cellar/opencv3/3.1.0_1/include/opencv;/usr/local/Cellar/opencv3/3.1.0_1/include" ) 
#INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} ) 

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

SUBDIRLIST(TRACKERS "${CMAKE_SOURCE_DIR}/trackers/")
FOREACH(tracker ${TRACKERS})
	SUBDIRS("trackers/${tracker}")
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/trackers/${tracker})
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${tracker})
	MESSAGE(STATUS "    tracker included: ${tracker}")
ENDFOREACH()

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