#include "../include/debugLayout.hpp"
#include <sstream>

DebugLayout::DebugLayout() :
    shader(Shader::loadShader("textShader")),
    font("./game/fonts/ConsolaMono-Bold.ttf", 0, 36) {

}

void DebugLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
    shader.use();

    shader.setUniform("projection", m_ortho);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Position and view direction
    std::stringstream builder;
    builder << "pos:" << pos << " ";
    builder << "yaw:" << formatFloat("%.2f", yaw) << " ";
    builder << "pitch:" << formatFloat("%.2f", pitch) << " ";
    font.RenderText(shader, builder.str(), 10, height - 20, 0.5, glm::vec3(0.f));
    // Selected block
    builder = std::stringstream();
    if (isSelectedBlock) {
        builder << "sel:" << selectedBlock << " ";
        builder << "face: " << WDirToString(selectedFace) << " ";
    }
    else
        builder << "-";
    font.RenderText(shader, builder.str(), 10, height - 38, 0.5, glm::vec3(0.f));
    // Player flags
    builder = std::stringstream();
    if (groundFlag)
        builder << "grounded ";
    if (flightmodFlag)
        builder << "flightmod ";
    font.RenderText(shader, builder.str(), 10, height - 56, 0.5, glm::vec3(0.f));
    // Perfomance
    builder = std::stringstream();
    builder << "fps: " << fps << " ftime: " << formatFloat("%.3f", 1.f / fps) << "ms ";
    font.RenderText(shader, builder.str(), width - 280, height - 20, 0.5, glm::vec3(0.f));
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

void DebugLayout::setSelectedBlock(const glm::ivec3 &block, WDir face, bool flag) {
    isSelectedBlock = flag;
    selectedBlock = block;
    selectedFace = face;
}

void DebugLayout::setFPS(uint fps) {
    this->fps = fps;
}