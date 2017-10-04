cmake_minimum_required (VERSION 3.2)
project(contours)

SET (MAIN_SOURCES
    contours2.cpp
    )

SET (SOURCES ${MAIN_SOURCES})

# define project's build target with project's source files
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE opencv)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET (PLATFORM x64)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET (PLATFORM x86)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

SET (INSTALL_DEST ${CMAKE_BINARY_DIR}/bin/${PLATFORM})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_world310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_world310.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Release)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_core310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_imgcodecs310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_imgproc310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_features2d310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_calib3d310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_highgui310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_video310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    install(FILES ${OPENCV_BIN_DIR}/Debug/opencv_videoio310d.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
    
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_core310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_imgcodecs310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_imgproc310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_features2d310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_calib3d310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_highgui310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_video310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
    install(FILES ${OPENCV_BIN_DIR}/Release/opencv_videoio310.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )