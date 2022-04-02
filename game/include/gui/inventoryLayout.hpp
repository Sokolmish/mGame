#ifndef INVENTORY_LAYOUT_H_
#define INVENTORY_LAYOUT_H_

#include "util/util.hpp"
#include "util/shader.hpp"
#include "item.hpp"
#include "configurations.hpp"
#include "player.hpp"
#include <vector>

class InventoryLayout {
private:
    struct Cell {
        float x1, y1, x2, y2;
        bool isSidebar;
        int index;
    };

    Shader tshader;
    GLuint vao[1], vbo[1];
    GLfloat *buff;
    int vertexCount;

    Player *player;

    std::vector<Cell> cells;
    Item pickedItem;

    const iventory_config::Config cfg;

public:
    explicit InventoryLayout(Player *player);
    ~InventoryLayout();

    void changeGeometry(float width, float height);
    void show(const glm::mat4 &m_ortho);

    void updateSidebar(const std::vector<Item> &items);
    void updateInventory(const std::vector<Item> &items);

    void click(float x, float y, int code, int action);
};

#endif
