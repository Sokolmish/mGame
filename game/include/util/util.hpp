#ifndef __UTIL_H__
#define __UTIL_H__

#include "glew.hpp"
#include "GLFW/glfw3.h"

#include "param.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdint.h>

// Enums

enum WDir { NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8, UP = 16, DOWN = 32 };
typedef uint32_t WMultiDir;
std::string WDirToString(WDir dir);

enum MouseButton { MOUSE_NONE, MOUSE_RIGHT, MOUSE_LEFT, MOUSE_MIDDLE };

// Formatters

std::string formatFloat(const std::string &format, float num);

// Printers

std::ostream& operator<<(std::ostream &os, const glm::vec2 &v);
std::ostream& operator<<(std::ostream &os, const glm::vec3 &v);
std::ostream& operator<<(std::ostream &os, const glm::vec4 &v);

std::ostream& operator<<(std::ostream &os, const glm::ivec2 &v);
std::ostream& operator<<(std::ostream &os, const glm::ivec3 &v);
std::ostream& operator<<(std::ostream &os, const glm::ivec4 &v);

// Special math

float fractf(float x);
int nfloor(float a);
int ndiv(int a, int b);
int nmod(int a, int b);

// Ray intersector

class RayIntersector {
private:
    glm::vec3 orig, dir;
    glm::vec3 invdir;
    int sign[3];
public:
    RayIntersector(const glm::vec3 &orig, const glm::vec3 &dir);
    bool intersect(const glm::vec3 &aa, const glm::vec3 &bb, WDir &face) const;
};

#endif
