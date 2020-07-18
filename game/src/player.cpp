#include "../include/player.h"
#include <cmath>

Player::Player() {
    halfSize = 0.3f;
    height = 1.8f;
    camHeight = 1.5f;

    pos = glm::vec3(0);
    yaw = pitch = 0;
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