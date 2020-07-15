cmake_minimum_required(VERSION 3.10)
project(mGame)
project(mGame VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
 
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/game/include/*.h"
    "${PROJECT_SOURCE_DIR}/game/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/game/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/game/src/*.c"
)

include_directories(
    ${PROJECT_SOURCE_DIR}/game/headers
    ${PROJECT_SOURCE_DIR}/game/src
)

add_executable(mGame ${all_SRCS})
target_link_libraries (mGame 
    ${OPENGL_LIBRARIES} 
    ${GLEW_LIBRARIES} 
    glfw ${GLFW_LIBRARIES} 
    ${FREETYPE_LIBRARIES}
)
