cmake_minimum_required (VERSION 3.2)
project(spylib C CXX)

SET (SOURCES OpenCVObjectsCommands.cpp OpenCVObjectsCommands.h)

# define project's build target with project's source files
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# add macros
add_definitions(-DSPYLIB_EXPORTS)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/json)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv)

SET (INSTALL_DEST ${CMAKE_BINARY_DIR}/bin/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.dll DESTINATION ${INSTALL_DEST} CONFIGURATIONS Debug)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.dll DESTINATION ${INSTALL_DEST} CONFIGURATIONS Release)