#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "util/util.hpp"
#include "camera.hpp"
#include "gameWorld.hpp"

class Player {
private:
    glm::vec3 pos;
    float yaw, pitch;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool flightMode;

    bool checkNewPos(const GameWorld &world, const glm::vec3 &pos) const;

    mutable char _cached = 0;

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

    void doPhysics(GLFWwindow *window, const GameWorld &world, float dt);
    bool isGrounded(const GameWorld &world) const;
    bool getSelectedBlock(const GameWorld &world, glm::ivec3 &block, WDir &face);
};

#endif