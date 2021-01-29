#include "../include/util/util.hpp"
#include "../include/player.hpp"
#include "../include/inputEvents.hpp"
#include <cmath>

Player::Player() {
    halfSize = 0.3f;
    height = 1.8f;
    camHeight = 1.5f;

    pos = glm::vec3(0.f);
    yaw = 0.f;
    pitch = 0.f;

    velocity = glm::vec3(0.f);
    acceleration = glm::vec3(0.f);
    flightMode = false;

    selectedItem = 0;
    sidebar = std::vector<Item>(9);
    inventory = std::vector<Item>(27);
}

// ...

bool Player::checkNewPos(const GameWorld &world, const glm::vec3 &pos) const {
    for (int yy = nfloorf(pos.y); yy <= pos.y + height; yy++) {
        for (int xx = nfloorf(pos.x - halfSize); xx <= pos.x + halfSize; xx++) {
            for (int zz = nfloorf(pos.z - halfSize); zz <= pos.z + halfSize; zz++) {
                if (world.checkBlock(xx, yy, zz)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Player::doPhysics(GLFWwindow *window, const GameWorld &world, float dt, glm::vec3 delta) {
    bool grounded = isGrounded(world);

    if (!isFlight()) {
        if (!grounded) {
            acceleration = glm::vec3(
                0.f,
                -9.81f,
                0.f
            );
        }
    }
    else {
        acceleration = glm::vec3(0.f);
        velocity = glm::vec3(0.f);
    }

    if (acceleration != glm::vec3(0.f)) {
        velocity += acceleration * dt;
    }
    if (velocity != glm::vec3(0.f)) {
        delta += velocity * dt;
    }

    int dimMoved = -1;
    float moveStep = 0.02f;
    while (glm::length(delta) >= moveStep) {
        glm::vec3 curDelta = moveStep * glm::normalize(delta);
        delta -= curDelta;
        dimMoved = 0;
        for (int i = 0; i < 3; i++) {
            glm::vec3 td(0.f);
            td[i] = curDelta[i];
            if (checkNewPos(world, pos + td)) {
                pos += td;
                dimMoved++;
            }
        }
        if (dimMoved == 0)
            break;
    }

    if (dimMoved != 0) {
        for (int i = 0; i < 3; i++) {
            glm::vec3 td(0.f);
            td[i] = delta[i];
            if (checkNewPos(world, pos + td))
                pos += td;
        }
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
    for (int xx = nfloorf(pos.x - halfSize); xx <= pos.x + halfSize; xx++) {
        for (int zz = nfloorf(pos.z - halfSize); zz <= pos.z + halfSize; zz++) {
            if (fractf(pos.y) < 2e-2f && world.checkBlock(xx, pos.y - 1, zz))
                return true;
            else if (fractf(pos.y) > 1.f - 2e-2f && world.checkBlock(xx, pos.y, zz))
                return true;
        }
    }
    return false;
}

// Camera getters

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

// Position getters/setters

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

// View angles getters/setters

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

// Velocity and acceleration getters/setters

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

// Other getters/setters 

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
