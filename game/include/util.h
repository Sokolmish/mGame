#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <glm/vec3.hpp>

enum WDir { NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8, UP = 16, DOWN = 32 };
typedef uint32_t WMultiDir;

std::string formatFloat(const std::string &format, float num);

std::ostream& operator<<(std::ostream &os, const glm::vec3 &v);

float fractf(float x);

#endif