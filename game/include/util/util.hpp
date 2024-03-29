#ifndef UTIL_H_
#define UTIL_H_

#include "glew.hpp"
#include "GLFW/glfw3.h"

#include "param.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <cstdint>
#include <stdexcept>

// Enums

enum WDir { NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8, UP = 16, DOWN = 32 };
typedef uint32_t WMultiDir;
std::string WDirToString(WDir dir);

enum MouseButton { MOUSE_NONE, MOUSE_RIGHT, MOUSE_LEFT, MOUSE_MIDDLE };

// Files

std::string readTextFile(const std::string &path);
void writeTextFile(const std::string &path, const std::string &text);

// String operations

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
