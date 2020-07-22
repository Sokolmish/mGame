#ifndef __GUI_LAYOUT_H__
#define __GUI_LAYOUT_H__

#include "shader.hpp"
#include <GL/gl.h>

class GuiLayout {
private:
    Shader shader;

    GLuint VAO, VBO;
public:
    GuiLayout();
    void show(float width, float height) const;
};

#endif