#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "util/util.hpp"
#include "camera.hpp"
#include "gameWorld.hpp"
#include "item.hpp"
#include <vector>

class Player {
private:
    glm::vec3 pos;
    float yaw, pitch;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool flightMode;

    bool checkNewPos(const GameWorld &world, const glm::vec3 &pos) const;
public:
    int selectedItem;
    std::vector<Item> sidebar;
    std::vector<Item> inventory;

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

    void doPhysics(const GameWorld &world, float dt, glm::vec3 delta);
    bool isGrounded(const GameWorld &world) const;
    bool getSelectedBlock(const GameWorld &world, glm::ivec3 &block, WDir &face);

    void selectItem(int item);
    int getSelectedCell() const;
    Item getSelectedItem() const;
};

#endif
