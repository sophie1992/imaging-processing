cmake_minimum_required(VERSION 3.1)
project(Project3DCV VERSION 1.0 LANGUAGES CXX)

# configure project to use OpenCV
set (OpenCV_STATIC off)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# set compiler options for Visual Studio
if (MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT;%(IgnoreSpecificDefaultLibraries)")
endif ()

# set variables with source files
set(DIR src)
set(HDR ${DIR}/Constants.hpp ${DIR}/tools.hpp ${DIR}/Calibration.hpp ${DIR}/Sequence.hpp ${DIR}/Tracking.hpp ${DIR}/Triangulation.hpp)
set(SRC ${DIR}/tools.cpp ${DIR}/main.cpp ${DIR}/Calibration.cpp ${DIR}/Sequence.cpp ${DIR}/Tracking.cpp ${DIR}/Triangulation.cpp)

# set up file tree in IDE
source_group("Source Files" FILES ${SRC})
source_group("Header Files" FILES ${HDR})

# create executable
add_executable(Project3DCV ${SRC} ${HDR})
target_link_libraries(Project3DCV ${OpenCV_LIBS})
