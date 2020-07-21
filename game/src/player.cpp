#include "../include/player.hpp"
#include <cmath>
#include "../include/inputEvents.hpp"
#include <glm/gtc/matrix_transform.hpp>

Player::Player() {
    halfSize = 0.3f;
    height = 1.8f;
    camHeight = 1.5f;

    pos = glm::vec3(0);
    yaw = pitch = 0;

    velocity = acceleration = glm::vec3(0);
    flightMode = false;
}

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
    _cached = 0b00;
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

bool Player::isGrounded(const Chunk &chunk) const {
    if ((_cached & 0b01) != 0)
        return (_cached & 0b10) != 0;

    glm::vec3 pos = getPos();
    for (int xx = pos.x - halfSize; xx <= pos.x + halfSize; xx++) {
        for (int zz = pos.z - halfSize; zz <= pos.z + halfSize; zz++) {
            if (fractf(pos.y) < 5e-3f && chunk.checkBlock(xx, pos.y - 1, zz)) {
                _cached = 0b11;
                return true;
            }
            else if (fractf(pos.y) > 1.f - 2e-3f && chunk.checkBlock(xx, pos.y, zz)) {
                _cached = 0b11;
                return true;
            }
        }
    }
    _cached = 0b01;
    return false;
}

bool Player::checkNewPos(const Chunk &chunk, const glm::vec3 &pos) const {
    if (pos.x - halfSize < 0 || pos.x + halfSize > 16)
        return false;
    if (pos.z - halfSize < 0 || pos.z + halfSize > 16)
        return false;
    for (int yy = pos.y; yy <= pos.y + height; yy++) {
        for (int xx = pos.x - halfSize; xx <= pos.x + halfSize; xx++) {
            for (int zz = pos.z - halfSize; zz <= pos.z + halfSize; zz++) {
                if (chunk.checkBlock(xx, yy, zz))
                    return false;
            }
        }
    }
    return true;
}

void Player::doPhysics(GLFWwindow *window, const Chunk &chunk, float dt) {
    glm::vec3 delta = InputPoller::pollMovement(window, *this, dt);
    glm::vec3 lastPos = getPos();

    bool grounded = isGrounded(chunk);
    
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
    while (glm::length(delta) >= 0.15) {
        glm::vec3 curDelta = 0.15f * glm::normalize(delta);
        delta -= curDelta;
        glm::vec3 pos3 = getPos();
        dimMoved = 0;
        for (int i = 0; i < 3; i++) {
            glm::vec3 td(0.f);
            td[i] = curDelta[i];
            if (checkNewPos(chunk, pos3 + td)) {
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
            if (checkNewPos(chunk, pos3 + td))
                pos3 += td;
        }
        setPos(pos3);
    }
}