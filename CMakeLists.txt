cmake_minimum_required(VERSION 3.17)
project(Tanks)

set(CMAKE_CXX_STANDARD 14)

include_directories ( /System/Library/Frameworks )
find_library(OpenGL_LIBRARY OpenGL )
set(EXTRA_LIBS ${OpenGL_LIBRARY})

add_executable(Tanks main.cpp)

find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(Tanks
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${PNG_LIBRARY}
)
