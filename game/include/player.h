#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "camera.h"
#include <glm/vec3.hpp>

class Player {
private:
    glm::vec3 pos;
    float yaw, pitch;
public:
    float halfSize;
    float height, camHeight;

    Player();
    Camera getCamera() const;
    glm::vec3 getViewDir() const;
    glm::vec3 getMoveDir() const;

    glm::vec3 getPos() const;
    void setPos(float x, float y, float z);
    void setPos(const glm::vec3 &np);
    void move(float dx, float dy, float dz);
    void move(const glm::vec3 &delta);

    void setYaw(float yaw);
    void setPitch(float pitch);
    float getYaw() const;
    float getPitch() const;
};

#endif