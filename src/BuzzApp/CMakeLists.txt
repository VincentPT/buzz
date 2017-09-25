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

SET (MAIN_SOURCES
    BuzzApp.cpp
    BuzzApp.h
    BuzzWindow.h
    BuzzWindow.cpp
    BuzzWindow_PartReadObjects.cpp
    Resources.h
    resources/Resources.rc
    resources/buzz.rc
    resources/resource.h
    )

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

SET (DIALOG_SOURCES
    dialogs/ObjectInputerDlg.cpp
    dialogs/ObjectInputerDlg.h
    dialogs/ObjectSettingsDlg.cpp
    dialogs/ObjectSettingsDlg.h
    dialogs/WindowSettingsDlg.cpp
    dialogs/WindowSettingsDlg.h
    dialogs/BuzzDialog.h
    dialogs/BuzzDialog.cpp
    dialogs/BuzzCustomGui.h
    dialogs/BuzzCustomGui.cpp
    dialogs/ObjectHierarchyDlg.h
    dialogs/ObjectHierarchyDlg.cpp
    dialogs/ObjectHierarchyDlg2.h
    dialogs/ObjectHierarchyDlg2.cpp
    dialogs/Win32Dlg.h
    dialogs/Win32Dlg.cpp
)

SET (UTILITIES
    utils/ClipboardViewer.h
    utils/ClipboardViewer.cpp
    utils/string_util.hpp
    utils/string_util.cpp
    utils/BuzzSpyClient.cpp
    utils/BuzzSpyClient.h
    ../spy-client/SpyClient.cpp
    ../spy-client/SpyClient.h
    ../spy-client/SpyClientUtils.hpp
)

SET (DRAWOBJS
    drawobjs/BuzzContainer.cpp
    drawobjs/BuzzContainer.h
    drawobjs/BuzzDrawObj.cpp
    drawobjs/BuzzDrawObj.h
    drawobjs/BuzzPoint.cpp
    drawobjs/BuzzPoint.h
    drawobjs/BuzzPolygon.cpp
    drawobjs/BuzzPolygon.h
    drawobjs/BuzzRect.cpp
    drawobjs/BuzzRect.h
    drawobjs/BuzzHasLine.cpp
    drawobjs/BuzzHasLine.h
)
    
SET (SOURCES ${MAIN_SOURCES} ${PRETZEL_SOURCES} ${DIALOG_SOURCES} ${UTILITIES} ${DRAWOBJS})

# [Visual studio specific]group the source into solution folder
source_group(PretzelGui FILES ${PRETZEL_SOURCES})
source_group(dialogs FILES ${DIALOG_SOURCES})
source_group(utils FILES ${UTILITIES})
source_group(drawobjs FILES ${DRAWOBJS})

# define project's build target with project's source files
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/cinder/blocks/PretzelGui/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/spy)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/spylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/json)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/spy-client)

target_link_libraries(${PROJECT_NAME} cinder Comctl32)
# set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT /NODEFAULTLIB:LIBCPMT" )
# target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)

add_dependencies(${PROJECT_NAME} spylib)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET (PLATFORM x64)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET (PLATFORM x86)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/spy/${PLATFORM}/$<CONFIGURATION>/spy-engine-1.0.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/spylib/$<CONFIGURATION>/spylib.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/)

SET (INSTALL_DEST ${CMAKE_BINARY_DIR}/bin/${PLATFORM}/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)
install(FILES ${CMAKE_SOURCE_DIR}/spy/${PLATFORM}/debug/spy-engine-1.0.dll DESTINATION ${INSTALL_DEST}/debug CONFIGURATIONS Debug)
install(FILES ${CMAKE_SOURCE_DIR}/spy/${PLATFORM}/release/spy-engine-1.0.dll DESTINATION ${INSTALL_DEST}/release CONFIGURATIONS Release)