#ifndef INTERFACE_LAYOUT_H_
#define INTERFACE_LAYOUT_H_

#include "util/util.hpp"
#include "util/shader.hpp"
#include "item.hpp"
#include "configurations.hpp"
#include "player.hpp"
#include <vector>

class InterfaceLayout {
private:
    Shader tshader;
    GLuint vao[1], vbo[1];
    float *buff;
    int vertexCount;

    int selectedCell;
    Player *player;

    const iventory_config::Config cfg;

public:
    explicit InterfaceLayout(Player *player);
    ~InterfaceLayout();

    void changeGeometry(float width, float height);
    void show(const glm::mat4 &m_ortho);

    void selectSidebarCell(int cellnum);
    void updateSidebarItems(const std::vector<Item> &items);
};

#endif
