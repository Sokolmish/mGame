cmake_minimum_required(VERSION 3.10)
project(mGame)
project(mGame VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -g")
endif ()

add_compile_options(-Wall)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

include_directories(
    ${PROJECT_SOURCE_DIR}/game/include
    ${PROJECT_SOURCE_DIR}/game/include/lib
)

file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/game/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/game/src/*.c"

    "${PROJECT_SOURCE_DIR}/game/src/*/*.cpp"
    "${PROJECT_SOURCE_DIR}/game/src/*/*.c"

    "${PROJECT_SOURCE_DIR}/game/src/lib/*/*.cpp"
    "${PROJECT_SOURCE_DIR}/game/src/lib/*/*.c"
    "${PROJECT_SOURCE_DIR}/game/src/lib/*/*.cc"
)

add_executable(mGame ${all_SRCS})
target_link_libraries (mGame
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)
