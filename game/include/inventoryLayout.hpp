#ifndef __INVENTORY_LAYOUT_H__
#define __INVENTORY_LAYOUT_H__

#include "util/util.hpp"
#include "util/shader.hpp"
#include "item.hpp"
#include <vector>

class InventoryLayout {
private:
    struct Cell {
        float x1, y1, x2, y2;
        const Item *item;
    };

    Shader cshader, tshader;
    GLuint vao[2], vbo[2];
    float *cbuff, *tbuff;

    std::vector<Item> sidebar;
    std::vector<Item> inventory;

    size_t fillSidebar(float *buff, uint width, uint height) const;
    size_t fillInventory(float *buff, uint width, uint height) const;

    std::vector<InventoryLayout::Cell> calcCells(uint width, uint height) const;

public:
    InventoryLayout();
    ~InventoryLayout();

    void updateSidebar(const std::vector<Item> &items);
    void updateInventory(const std::vector<Item> &items);

    void show(const glm::mat4 &m_ortho, float width, float height) const;
};

#endif