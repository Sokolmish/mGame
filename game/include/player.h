#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "camera.h"
#include <glm/vec3.hpp>

class Player {
private:
    glm::vec3 pos;
    float yaw, pitch;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool flightMode;
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

    glm::vec3 getVelocity() const;
    glm::vec3 getAcceleration() const;
    void setVelocity(const glm::vec3 &v);
    void setAcceleration(const glm::vec3 &a);

    bool isFlight() const;
    void setFlight(bool flight);
};

#endif