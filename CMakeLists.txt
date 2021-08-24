cmake_minimum_required(VERSION 3.17)
project(Tanks)

set(CMAKE_CXX_STANDARD 14)

include_directories ( /System/Library/Frameworks )
find_library(OpenGL_LIBRARY OpenGL )
set(EXTRA_LIBS ${OpenGL_LIBRARY})

add_executable(Tanks main.cpp map.cpp projectile.cpp tank.cpp)

#target_link_libraries(HelloWorld PNG::PNG)
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

#include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(Tanks
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARY}
        ${PNG_LIBRARY})

#link_libraries(${Executable}
#    Threads::Threads
#    OpenGL::OpenGL
#    OpenGL::GL
#    OpenGL::GLX
#    PNG::PNG
#)
