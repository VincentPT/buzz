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

if (${PLATFORM} STREQUAL "x64")
    find_library (OPENCV_WORLD_D opencv_world310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
    find_library (OPENCV_WORLD_R opencv_world310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)

    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${OPENCV_WORLD_D}" INTERFACE optimized "${OPENCV_WORLD_R}")
else ()
    find_library (opencv_calib_d opencv_calib3d310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
    find_library (opencv_calib_r opencv_calib3d310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)
    find_library (opencv_core_d opencv_core310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
    find_library (opencv_core_r opencv_core310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)
    find_library (opencv_features_d opencv_features2d310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
    find_library (opencv_features_r opencv_features2d310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)
    find_library (opencv_imgcodecs_d opencv_imgcodecs310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
    find_library (opencv_imgcodecs_r opencv_imgcodecs310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)
    find_library (opencv_imgproc_d opencv_imgproc310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
    find_library (opencv_imgproc_r opencv_imgproc310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)
    find_library (opencv_highgui_d opencv_highgui310d PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES debug)
    find_library (opencv_highgui_r opencv_highgui310 PATHS ${I_OPENCV_LIB_DIR} PATH_SUFFIXES release)
    
    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${opencv_calib_d}" INTERFACE optimized "${opencv_calib_r}")
    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${opencv_core_d}" INTERFACE optimized "${opencv_core_r}")
    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${opencv_features_d}" INTERFACE optimized "${opencv_features_r}")
    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${opencv_imgcodecs_d}" INTERFACE optimized "${opencv_imgcodecs_r}")
    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${opencv_imgproc_d}" INTERFACE optimized "${opencv_imgproc_r}")
    target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${opencv_highgui_d}" INTERFACE optimized "${opencv_highgui_r}")
endif ()