#ifndef __GUI_LAYOUT_H__
#define __GUI_LAYOUT_H__

#include "util/util.hpp"
#include "util/shader.hpp"
#include "interface.hpp"

class GuiLayout {
private:
    Shader shader;
    GLuint VAO, VBO;
    Interface *layout;
public:
    GuiLayout(Interface *layout);
    void show(const glm::mat4 &m_ortho, float width, float height) const;
};

#endif