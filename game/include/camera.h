#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/vec3.hpp>

struct Camera {
    glm::vec3 pos;
    float yaw, pitch, roll;
    float zoom;

    Camera() : 
        pos(glm::vec3(0.f)), yaw(0), pitch(0), roll(0), zoom(1.f) {}
    Camera(const glm::vec3 &pos, float yaw, float pitch) : 
        pos(pos), yaw(yaw), pitch(pitch), roll(0), zoom(1.f) {} 
};

#endif