#include "util/util.hpp"
#include <cmath>
#include <sstream>
#include <fstream>
#include "fmt/ostream.h"

//
// Enums
//

std::string WDirToString(WDir dir) {
    if (dir == NORTH)       return "North";
    else if (dir == SOUTH)  return "South";
    else if (dir == EAST)   return "East";
    else if (dir == WEST)   return "West";
    else if (dir == UP)     return "Up";
    else if (dir == DOWN)   return "Down";
    else                    return "error_WDir";
}

//
// Files
//

std::string readTextFile(const std::string &path) {
    // TODO: check for better methods
    std::ifstream in;
    in.open(path);
    std::stringstream ss;
    ss << in.rdbuf();
    in.close();
    return ss.str();
}

void writeTextFile(const std::string &path, const std::string &text) {
    std::ofstream ofs(path);
    ofs << text << std::endl;
    ofs.close();
}


//
// Printers
//

std::ostream& operator<<(std::ostream &os, const glm::vec2 &v) {
    fmt::print(os, "({:.2f};{:.2f})", v.x, v.y);
    return os;
}

std::ostream& operator<<(std::ostream &os, const glm::vec3 &v) {
    fmt::print(os, "({:.2f};{:.2f};{:.2f})", v.x, v.y, v.z);
    return os;
}

std::ostream& operator<<(std::ostream &os, const glm::vec4 &v) {
    fmt::print(os, "({:.2f};{:.2f};{:.2f};{:.2f})", v.x, v.y, v.z, v.w);
    return os;
}


std::ostream& operator<<(std::ostream &os, const glm::ivec2 &v) {
    fmt::print(os, "({};{})", (int)v.x, (int)v.y);
    return os;
}

std::ostream& operator<<(std::ostream &os, const glm::ivec3 &v) {
    fmt::print(os, "({};{};{})", (int)v.x, (int)v.y, (int)v.z);
    return os;
}

std::ostream& operator<<(std::ostream &os, const glm::ivec4 &v) {
    fmt::print(os, "({};{};{};{})", (int)v.x, (int)v.y, (int)v.z, (int)v.w);
    return os;
}

//
// Special math
//

float fractf(float x) {
    float wh;
    return modff(x, &wh);
}

int nfloorf(float a) {
    if (a < 0) return (int)(a - 1);
    else return (int)a;
}

int eucl_div(int a, int b) {
    if (a >= 0) return a / b;
    else return (a - b + 1) / b;
}

int eucl_mod(int a, int b) {
    return (b + (a % b)) % b;
}

//
// Ray intersector
//

RayIntersector::RayIntersector(const glm::vec3 &orig, const glm::vec3 &dir) : orig(orig), dir(dir) {
    invdir = 1.f / dir;
    sign[0] = (invdir.x < 0) ? 1 : 0;
    sign[1] = (invdir.y < 0) ? 1 : 0;
    sign[2] = (invdir.z < 0) ? 1 : 0;
}

inline WDir getIntersectionFace(const glm::vec3 point, const glm::vec3 &aa, const glm::vec3 &bb) {
    float temp;
    float min = fabsf(point.x - bb.x);
    WDir face = EAST;
    temp = fabsf(point.x - aa.x);
    if (min > temp) {
        min = temp;
        face = WEST;
    }
    temp = fabsf(point.z - bb.z);
    if (min > temp) {
        min = temp;
        face = SOUTH;
    }
    temp = fabsf(point.z - aa.z);
    if (min > temp) {
        min = temp;
        face = NORTH;
    }
    temp = fabsf(point.y - bb.y);
    if (min > temp) {
        min = temp;
        face = UP;
    }
    temp = fabsf(point.y - aa.y);
    if (min > temp) {
        min = temp;
        face = DOWN;
    }
    return face;
}

bool RayIntersector::intersect(const glm::vec3 &aa, const glm::vec3 &bb, WDir &face) const {
    float tmin, tmax, tymin, tymax, tzmin, tzmax, t;
    auto bounds = std::array{ aa, bb };

    tmin = (bounds[sign[0]].x - orig.x) * invdir.x;
    tmax = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
    tymin = (bounds[sign[1]].y - orig.y) * invdir.y;
    tymax = (bounds[1 - sign[1]].y - orig.y) * invdir.y;

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

    t = tmin;
    if (t < 0) {
        t = tmax;
        if (t < 0)
            return false;
    }

    face = getIntersectionFace(orig + t * dir, aa, bb);
    return true;
}
