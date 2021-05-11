#include "gui/debugLayout.hpp"
#include <sstream>
#include "fmt/core.h"
#include "fmt/ostream.h"

DebugLayout::DebugLayout() {
    shader = Shader::getShader("textShader");
    font = new Font("./game/fonts/ConsolaMono-Bold.ttf", 0, 36);
}

DebugLayout::~DebugLayout() {
    delete font;
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
    fmt::print(builder, "yaw:{:.2f} ", yaw);
    fmt::print(builder, "pitch:{:.2f} ", pitch);
    if (315 <= yaw || yaw <= 45)
        builder << "face: North ";
    else if (45 <= yaw && yaw <= 135)
        builder << "face: East";
    else if (135 <= yaw && yaw <= 225)
        builder << "face: South";
    else
        builder << "face: West";
    font->RenderText(shader, builder.str(), 10, height - 20, 0.5, glm::vec3(0.f));
    // Selected block
    builder = std::stringstream();
    if (isSelectedBlock) {
        builder << "sel:" << selectedBlock << " ";
        builder << "face: " << WDirToString(selectedFace) << " ";
    }
    else
        builder << "-";
    font->RenderText(shader, builder.str(), 10, height - 38, 0.5, glm::vec3(0.f));
    // Player flags
    builder = std::stringstream();
    if (groundFlag)
        builder << "grounded ";
    if (flightmodFlag)
        builder << "flightmod ";
    font->RenderText(shader, builder.str(), 10, height - 56, 0.5, glm::vec3(0.f));
    // Perfomance
    builder = std::stringstream();
    fmt::print(builder, "fps: {} ftime: {:.3f}ms ", fps, 1.f / fps);
    font->RenderText(shader, builder.str(), width - 280, height - 20, 0.5, glm::vec3(0.f));
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
