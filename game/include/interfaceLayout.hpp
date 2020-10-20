#ifndef __INTERFACE_LAYOUT_H__
#define __INTERFACE_LAYOUT_H__

#include "util/util.hpp"
#include "util/shader.hpp"
#include "item.hpp"
#include <vector>

class InterfaceLayout {
private:
    Shader cshader, tshader;
    GLuint vao[2], vbo[2];
    float *buff;
    int vertexCount;

    int selectedCell;
    std::vector<Item> sidebar;

    size_t fillCrosshair(float *buff, uint width, uint height) const;
    size_t fillSidebar(float *buff, uint width, uint height) const;
    // size_t fillCells(float *buff, uint width, uint height) const;
public:
    InterfaceLayout();
    ~InterfaceLayout();

    void show(const glm::mat4 &m_ortho, float width, float height) const;

    void selectSidebarCell(int cellnum);
    void updateSidebarItems(const std::vector<Item> &items);
};

#endif