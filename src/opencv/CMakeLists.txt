cmake_minimum_required(VERSION 3.2.0)

SET (TARGET_INTERFACE opencv)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set (PLATFORM x64)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set (PLATFORM x86)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# set include directories
SET (I_OPENCV_INCLUDE_DIR "")
SET (I_OPENCV_LIB_DIR "")

# if OPENCV_INCLUDE_DIR is defined, set include dir
if (DEFINED OPENCV_INCLUDE_DIR)
    SET(I_OPENCV_INCLUDE_DIR ${OPENCV_INCLUDE_DIR})
endif()

# if OPENCV_LIB_DIR is defined, set library dir
if (DEFINED OPENCV_LIB_DIR)
    SET(I_OPENCV_LIB_DIR ${OPENCV_LIB_DIR})
endif()

# apply the target to cmake
add_library(${TARGET_INTERFACE} INTERFACE)

# apply opencv inlcude dir to cmake
if (NOT ${I_OPENCV_INCLUDE_DIR} EQUAL "")
    target_include_directories(${TARGET_INTERFACE} INTERFACE ${I_OPENCV_INCLUDE_DIR})
endif()

message ("Platform: " ${PLATFORM})

find_library (OPENCV_WORLD_D opencv_world310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
find_library (OPENCV_WORLD_R opencv_world310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)

if (${PLATFORM} STREQUAL "x64")    
    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${OPENCV_WORLD_D}" INTERFACE optimized "${OPENCV_WORLD_R}")
endif ()