#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/vec3.hpp>

struct Camera {
    glm::vec3 pos;
    float yaw, pitch, roll;
    float zoom = 1.f;
};

#endif