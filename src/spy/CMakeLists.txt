cmake_minimum_required (VERSION 3.2)
project(spy C CXX)

SET (SOURCES 
    CustomCommand.cpp
    CustomCommand.h
    CustomCommandInvoker.cpp
    CustomCommandInvoker.h
    main.cpp
    Spy.cpp
    Spy.h
    spy_interfaces.h
)

# define project's build target with project's source files
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# add macros
add_definitions(-DSPY_EXPORTS)

target_link_libraries(${PROJECT_NAME} PRIVATE spylib PRIVATE Shlwapi)

SET (INSTALL_DEST ${CMAKE_BINARY_DIR}/bin/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.dll DESTINATION ${INSTALL_DEST} CONFIGURATIONS Debug)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.dll DESTINATION ${INSTALL_DEST} CONFIGURATIONS Release)