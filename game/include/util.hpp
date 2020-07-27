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

enum WDir { NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8, UP = 16, DOWN = 32 };
typedef uint32_t WMultiDir;
std::string WDirToString(WDir dir);

std::string formatFloat(const std::string &format, float num);

std::ostream& operator<<(std::ostream &os, const glm::vec3 &v);

std::ostream& operator<<(std::ostream &os, const glm::ivec3 &v);

float fractf(float x);

class RayIntersector {
private:
    glm::vec3 orig, dir;
    glm::vec3 invdir; 
    int sign[3]; 
public:
    RayIntersector(const glm::vec3 &orig, const glm::vec3 &dir);
    bool intersect(const glm::vec3 &aa, const glm::vec3 &bb, WDir &face) const;
};

bool initGLFW(GLFWwindow *&window);
bool initGLEW();

#endif