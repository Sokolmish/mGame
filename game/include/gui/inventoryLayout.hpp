#ifndef __INVENTORY_LAYOUT_H__
#define __INVENTORY_LAYOUT_H__

#include "../util/util.hpp"
#include "../util/shader.hpp"
#include "../item.hpp"
#include "../configurations.hpp"
#include <vector>

class InventoryLayout {
private:
    struct Cell {
        float x1, y1, x2, y2;
        const Item *item;
    };

    Shader tshader;
    GLuint vao[1], vbo[1];
    GLfloat *buff;
    int vertexCount;

    std::vector<Item> sidebar;
    std::vector<Item> inventory;

    const iventory_config::Config cfg;

public:
    InventoryLayout();
    ~InventoryLayout();

    void show(const glm::mat4 &m_ortho, float width, float height) const;

    void updateSidebar(const std::vector<Item> &items);
    void updateInventory(const std::vector<Item> &items);
};

#endif
