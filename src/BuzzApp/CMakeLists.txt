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

# define project's build target with project's source files
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

target_link_libraries(${PROJECT_NAME} cinder)
# set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT /NODEFAULTLIB:LIBCPMT" )
# target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)

SET (INSTALL_DEST ${CMAKE_BINARY_DIR}/bin/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST} CONFIGURATIONS Debug)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.exe DESTINATION ${INSTALL_DEST} CONFIGURATIONS Release)    