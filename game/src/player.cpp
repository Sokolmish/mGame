#include "../include/util/util.hpp"
#include "../include/player.hpp"
#include "../include/inputEvents.hpp"
#include <cmath>

Player::Player() {
    halfSize = 0.3f;
    height = 1.8f;
    camHeight = 1.5f;

    pos = glm::vec3(0);
    yaw = pitch = 0;

    velocity = acceleration = glm::vec3(0);
    flightMode = false;

    selectedItem = 0;
    sidebar = std::vector<Item>(9);
    inventory = std::vector<Item>(27);
}

// ...

bool Player::checkNewPos(const GameWorld &world, const glm::vec3 &pos) const {
    for (int yy = nfloor(pos.y); yy <= pos.y + height; yy++) {
        for (int xx = nfloor(pos.x - halfSize); xx <= pos.x + halfSize; xx++) {
            for (int zz = nfloor(pos.z - halfSize); zz <= pos.z + halfSize; zz++) {
                if (world.checkBlock(xx, yy, zz))
                    return false;
            }
        }
    }
    return true;
}

void Player::doPhysics(GLFWwindow *window, const GameWorld &world, float dt) {
    glm::vec3 delta = InputPoller::pollMovement(window, *this, dt);
    glm::vec3 lastPos = getPos();

    bool grounded = isGrounded(world);
    
    if (!isFlight()) {
        if (!grounded) {
            setAcceleration(glm::vec3(0, -9.81f, 0));
        }
        else {  
            setAcceleration(glm::vec3(0));
            setVelocity(glm::vec3(0));
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                setVelocity(glm::vec3(0, 4.85, 0));
        }
    }
    else {
        setAcceleration(glm::vec3(0));
        setVelocity(glm::vec3(0));
    }

    if (getAcceleration() != glm::vec3(0)) {
        glm::vec3 dvel = getVelocity() + getAcceleration() * dt;
        setVelocity(dvel);
    }
    if (getVelocity() != glm::vec3(0.f)) {
        glm::vec3 dpos = getVelocity() * dt;
        delta += dpos;
    }

    int dimMoved = -1;
    // TODO: ...
    while (glm::length(delta) >= 0.1) {
        glm::vec3 curDelta = 0.15f * glm::normalize(delta);
        delta -= curDelta;
        glm::vec3 pos3 = getPos();
        dimMoved = 0;
        for (int i = 0; i < 3; i++) {
            glm::vec3 td(0.f);
            td[i] = curDelta[i];
            if (checkNewPos(world, pos3 + td)) {
                pos3 += td;
                dimMoved++;
            }
        }
        setPos(pos3);
        if (dimMoved == 0)
            break;
    }

    if (dimMoved != 0) {
        glm::vec3 pos3 = lastPos;
        for (int i = 0; i < 3; i++) {
            glm::vec3 td(0.f);
            td[i] = delta[i];
            if (checkNewPos(world, pos3 + td))
                pos3 += td;
        }
        setPos(pos3);
    }
}

bool Player::getSelectedBlock(const GameWorld &world, glm::ivec3 &block, WDir &face) {
    const float maxlen = 3;
    glm::vec3 orig = getCamera().pos;
    glm::vec3 dir = getViewDir();
    RayIntersector ray(orig, dir);
    int dx = dir.x > 0 ? 1 : -1;
    int dy = dir.y > 0 ? 1 : -1;
    int dz = dir.z > 0 ? 1 : -1;
    for (int xx = 0; xx <= maxlen; xx++) {
        for (int zz = 0; zz <= maxlen; zz++) {
            for (int yy = 0; yy <= maxlen; yy++) {
                glm::ivec3 cur = glm::floor(orig + glm::vec3{ xx * dx, yy * dy, zz * dz });
                if (world.checkBlock(cur) && ray.intersect(cur, cur + glm::ivec3(1.f), face)) {
                    block = cur;
                    return true;
                }
            }
        }
    }
    return false;
}


bool Player::isGrounded(const GameWorld &world) const {
    glm::vec3 pos = getPos();
    for (int xx = nfloor(pos.x - halfSize); xx <= pos.x + halfSize; xx++) {
        for (int zz = nfloor(pos.z - halfSize); zz <= pos.z + halfSize; zz++) {
            if (fractf(pos.y) < 5e-3f && world.checkBlock(xx, pos.y - 1, zz))
                return true;
            else if (fractf(pos.y) > 1.f - 2e-3f && world.checkBlock(xx, pos.y, zz))
                return true;
        }
    }
    return false;
}

// ...

Camera Player::getCamera() const {
    return Camera(
        glm::vec3(pos.x, pos.y + camHeight, pos.z),
        yaw, pitch   
    );
}

glm::vec3 Player::getViewDir() const {
    return glm::vec3(cosf(pitch) * sinf(yaw), sinf(pitch), -cosf(pitch) * cosf(yaw));
}

glm::vec3 Player::getMoveDir() const {
    return glm::vec3(-sinf(yaw), 0, cosf(yaw));
}

glm::vec3 Player::getPos() const {
    return pos;
}

void Player::setPos(const glm::vec3 &np) {
    pos = np;
}

void Player::setPos(float x, float y, float z) {
    setPos(glm::vec3(x, y, z));
}

void Player::move(float dx, float dy, float dz) {
    move(glm::vec3(dx, dy, dz));
}

void Player::move(const glm::vec3 &delta) {
    setPos(pos + delta);
}

void Player::setYaw(float yaw) {
    this->yaw = yaw;
}

void Player::setPitch(float pitch) {
    this->pitch = pitch;
}

float Player::getYaw() const {
    return yaw;
}

float Player::getPitch() const {
    return pitch;
}

glm::vec3 Player::getVelocity() const {
    return velocity;
}

glm::vec3 Player::getAcceleration() const {
    return acceleration;
}

void Player::setVelocity(const glm::vec3 &v) {
    velocity = v;
}

void Player::setAcceleration(const glm::vec3 &a) {
    acceleration = a;
}

bool Player::isFlight() const {
    return flightMode;
}

void Player::setFlight(bool flight) {
    flightMode = flight;
}

void Player::selectItem(int item) {
    this->selectedItem = item;
}

int Player::getSelectedCell() const {
    return selectedItem;
}

Item Player::getSelectedItem() const {
    return sidebar[selectedItem];
}
