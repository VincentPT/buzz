cmake_minimum_required (VERSION 3.2)
project(buzz)

if (MSVC)    
    # Override the default /MD with /MT
    foreach( 
        flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE    )
        if( ${flag_var} MATCHES "/MD" )
            string( REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}" )
        endif()
    endforeach()
endif()

SET (SOURCES
    BuzzApp.cpp
    BuzzApp.h
    BuzzWindow.h
    BuzzWindow.cpp
    Resources.h
    resources/Resources.rc)

SET (PRETZEL_SOURCES    
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/BasePretzel.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/BasePretzel.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/PretzelRoot.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/PretzelRoot.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/PretzelRow.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/PretzelRow.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/ScrollPane.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/ScrollPane.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/WindowSystem.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/components/WindowSystem.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelButton.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelButton.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelColorPicker.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelColorPicker.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelEnum.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelEnum.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelLabel.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelLabel.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelSaveLoad.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelSaveLoad.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelShaders.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelSlider.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelSlider.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelTextField.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelTextField.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelToggle.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/modules/PretzelToggle.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/pretzel/PretzelGlobal.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/pretzel/PretzelGlobal.h
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/pretzel/PretzelGui.cpp
    ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src/pretzel/PretzelGui.h
    )

# [Visual studio specific]group the source into solution folder
source_group(PretzelGui FILES ${PRETZEL_SOURCES})

# define project's build target with project's source files
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${PRETZEL_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src)
target_link_libraries(${PROJECT_NAME} cinder)
# set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT /NODEFAULTLIB:LIBCPMT" )
# target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)

SET (INSTALL_DEST ${CMAKE_BINARY_DIR}/bin/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST} CONFIGURATIONS Debug)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST} CONFIGURATIONS Release)    