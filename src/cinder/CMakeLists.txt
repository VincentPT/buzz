cmake_minimum_required(VERSION 3.2.0)

SET (TARGET_INTERFACE cinder)

# set include directories
SET (I_CINDER_INCLUDE_DIR "")
SET (I_CINDER_LIB_DIR "")

# if CINDER_INCLUDE_DIR is defined, set include dir
if (DEFINED CINDER_INCLUDE_DIR)
    SET(I_CINDER_INCLUDE_DIR ${CINDER_INCLUDE_DIR})
endif()

# if CINDER_LIB_DIR is defined, set library dir
if (DEFINED CINDER_LIB_DIR)
    SET(I_CINDER_LIB_DIR ${CINDER_LIB_DIR})
endif()

# apply the target to cmake
add_library(${TARGET_INTERFACE} INTERFACE)

# apply opencv inlcude dir to cmake
if (NOT ${I_CINDER_INCLUDE_DIR} EQUAL "")
    target_include_directories(${TARGET_INTERFACE} INTERFACE ${I_CINDER_INCLUDE_DIR})
endif()

find_library (CINDER_WORLD_D cinder PATHS ${I_CINDER_LIB_DIR} PATH_SUFFIXES debug)
find_library (CINDER_WORLD_R cinder PATHS ${I_CINDER_LIB_DIR} PATH_SUFFIXES release)

target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${CINDER_WORLD_D}" INTERFACE optimized "${CINDER_WORLD_R}")