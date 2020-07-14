#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <glm/vec3.hpp>

std::string formatFloat(const std::string &format, float num) {
    char str[16];
    snprintf(str, 16, format.c_str(), num);
    return std::string(str);
}

std::ostream& operator<<(std::ostream &os, const glm::vec3 &v) {
    os << "(" << formatFloat("%.2f", v.x) << ";" << formatFloat("%.2f", v.y) << ";" << formatFloat("%.2f", v.z) << ")";
    return os;
}

#endif