#ifndef __UTIL_H__
#define __UTIL_H__

#include "../lib/glew.hpp"
#include "GLFW/glfw3.h"

#include "param.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <stdint.h>
#include <stdexcept>

// Enums

enum WDir { NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8, UP = 16, DOWN = 32 };
typedef uint32_t WMultiDir;
std::string WDirToString(WDir dir);

enum MouseButton { MOUSE_NONE, MOUSE_RIGHT, MOUSE_LEFT, MOUSE_MIDDLE };

// String operations

template<typename ...Args>
std::string str_format(const std::string& format, Args ...args) { // https://stackoverflow.com/a/26221725
    int size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if(size <= 0) {
        throw std::runtime_error("Error during formatting.");
    }
    char *buf = new char[size]; // TODO: also use static allocated buffer
    snprintf(buf, size, format.c_str(), args...);
    std::string res(buf, buf + size - 1); // We don't want the '\0' inside
    delete[] buf;
    return res;
}

std::ostream& operator<<(std::ostream &os, const glm::vec2 &v);
std::ostream& operator<<(std::ostream &os, const glm::vec3 &v);
std::ostream& operator<<(std::ostream &os, const glm::vec4 &v);

std::ostream& operator<<(std::ostream &os, const glm::ivec2 &v);
std::ostream& operator<<(std::ostream &os, const glm::ivec3 &v);
std::ostream& operator<<(std::ostream &os, const glm::ivec4 &v);

// Special math

float fractf(float x);
int nfloorf(float a);
int eucl_div(int a, int b);
int eucl_mod(int a, int b);

// Ray intersector

class RayIntersector {
private:
    glm::vec3 orig, dir;
    glm::vec3 invdir;
    char sign[3]; // Accepts only 0 or 1 values
public:
    RayIntersector(const glm::vec3 &orig, const glm::vec3 &dir);
    bool intersect(const glm::vec3 &aa, const glm::vec3 &bb, WDir &face) const;
};

#endif
