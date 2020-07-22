#include "../include/debugLayout.hpp"
#include "../include/util.hpp"
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

DebugLayout::DebugLayout() :
    font("./game/fonts/ConsolaMono-Bold.ttf", 0, 36),
    shader(Shader::loadShader("textShader")) {

}

void DebugLayout::show(float width, float height) const {
    shader.use();

    glm::mat4 proj = glm::ortho(0.0f, width, 0.0f, height);
    shader.setUniform("projection", proj);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    std::stringstream statusSS;
    statusSS << "pos=" << pos << ";";
    statusSS << "yaw=" << formatFloat("%.2f", yaw) << ";";
    statusSS << "pitch=" << formatFloat("%.2f", pitch) << ";";
    font.RenderText(shader, statusSS.str(), 10, height - 20, 0.5, glm::vec3(0.f));
    statusSS = std::stringstream();
    if (groundFlag)
        statusSS << "grounded;";
    if (flightmodFlag)
        statusSS << "flightmod;";
    font.RenderText(shader, statusSS.str(), 10, height - 38, 0.5, glm::vec3(0.f));
}

void DebugLayout::setPos(const glm::vec3 &pos) {
    this->pos = pos;
}

void DebugLayout::setPos(float x, float y, float z) {
    this->pos = glm::vec3(x, y, z);
}

void DebugLayout::setView(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
}

void DebugLayout::setGrounded(bool flag) {
    groundFlag = flag;
}

void DebugLayout::setFlightmoded(bool flag) {
    flightmodFlag = flag;
}
