cmake_minimum_required (VERSION 3.2)
project(buzz)

# add cinder depencies interface
add_subdirectory(cinder)

# define project's build target with project's source files
add_subdirectory(BuzzApp)