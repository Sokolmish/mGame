#include "../include/util.hpp"
#include <cmath>
#include <array>


std::string formatFloat(const std::string &format, float num) {
    char str[16];
    snprintf(str, 16, format.c_str(), num);
    return std::string(str);
}

std::ostream& operator<<(std::ostream &os, const glm::vec3 &v) {
    os << "(" << formatFloat("%.2f", v.x) << ";" << formatFloat("%.2f", v.y) << ";" << formatFloat("%.2f", v.z) << ")";
    return os;
}

std::ostream& operator<<(std::ostream &os, const glm::ivec3 &v) {
    os << "(" << (int)v.x << ";" << (int)v.y << ";" << (int)v.z << ")";
    return os;
}

float fractf(float x) {
    float wh;
    return modff(x, &wh);
}

RayIntersector::RayIntersector(const glm::vec3 &orig, const glm::vec3 &dir) : orig(orig), dir(dir) {
    invdir = 1.f / dir;
    sign[0] = (invdir.x < 0) ? 1 : 0;
    sign[1] = (invdir.y < 0) ? 1 : 0;
    sign[2] = (invdir.z < 0) ? 1 : 0;
}

bool RayIntersector::intersect(const glm::vec3 &aa, const glm::vec3 &bb) const {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    auto bounds = std::array{ aa, bb };

    tmin = (bounds[sign[0]].x - orig.x) * invdir.x;
    tmax = (bounds[1-sign[0]].x - orig.x) * invdir.x;
    tymin = (bounds[sign[1]].y - orig.y) * invdir.y;
    tymax = (bounds[1-sign[1]].y - orig.y) * invdir.y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[sign[2]].z - orig.z) * invdir.z;
    tzmax = (bounds[1-sign[2]].z - orig.z) * invdir.z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}