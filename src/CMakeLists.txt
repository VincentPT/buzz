cmake_minimum_required (VERSION 3.2)
project(buzz)

# add cinder depencies interface
add_subdirectory(cinder)

# add opencv depencies interface
add_subdirectory(opencv)

# spylib
add_subdirectory(spylib)

# buzz spy application
add_subdirectory(BuzzApp)

# host application
add_subdirectory(contours)