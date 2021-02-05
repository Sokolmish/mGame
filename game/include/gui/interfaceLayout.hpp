#ifndef __INTERFACE_LAYOUT_H__
#define __INTERFACE_LAYOUT_H__

#include "../util/util.hpp"
#include "../util/shader.hpp"
#include "../item.hpp"
#include "../configurations.hpp"
#include <vector>

class InterfaceLayout {
private:
    Shader tshader;
    GLuint vao[1], vbo[1];
    float *buff;
    int vertexCount;

    int selectedCell;
    std::vector<Item> sidebar;

    const iventory_config::Config cfg;

    mutable float lastWidth;
    mutable float lastHeight;
    mutable int lastSelected;

public:
    InterfaceLayout();
    ~InterfaceLayout();

    void show(const glm::mat4 &m_ortho, float width, float height) const;

    void selectSidebarCell(int cellnum);
    void updateSidebarItems(const std::vector<Item> &items);
};

#endif
