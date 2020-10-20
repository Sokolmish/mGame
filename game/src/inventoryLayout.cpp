#include "../include/inventoryLayout.hpp"
#include "../include/util/image.hpp"

#define SZ_FLT sizeof(float)

void fillColors1(float *collb) {
    glm::vec4 gray_back = { 0.6f, 0.6f, 0.6f, 1.0f };
    glm::vec4 gray_cell = { 0.35f, 0.35f, 0.35f, 1.0f };
    // glm::vec4 gray_highlight = { 0.85f, 0.85f, 0.85f, 1.0f };

    // Sidebar background
    for (uint i = 0; i < 6; i++)
        for (uint j = 0; j < 4; j++)
            collb[(0 + i) * 4 + j] = gray_back[j];

    // Sidebar cells
    for (uint i = 0; i < 54; i++) // 54 = 6 * 9
        for (uint j = 0; j < 4; j++)
            collb[(6 + i) * 4 + j] = gray_cell[j];

    // Inventory background
    for (uint i = 0; i < 6; i++)
        for (uint j = 0; j < 4; j++)
            collb[(60 + i) * 4 + j] = gray_back[j];

    // Inventory cells
    for (uint i = 0; i < 162; i++) // 162 = 6 * 9 * 3
        for (uint j = 0; j < 4; j++)
            collb[(66 + i) * 4 + j] = gray_cell[j];
}

std::vector<InventoryLayout::Cell> InventoryLayout::calcCells(uint width, uint height) const {
    std::vector<InventoryLayout::Cell> cells = std::vector<InventoryLayout::Cell>(0);

    float cy = height / 2.f;            // Vertical center
    float csz = 50;                     // Cell size
    float cmg = 7;                      // Cell margin
    float slef = csz + cmg + cmg + 20;  // Left pos

    // Sidebar
    float sh = (csz + cmg) * 9 + cmg;   // Sidebar total height
    float sbot = cy - sh / 2.f;         // Sidebar bottom pos
    for (int j = 8; j >= 0; j--) {
        float yoff = sbot + cmg + j * (csz + cmg);
        cells.push_back({ cmg, yoff, cmg + csz, yoff + csz, &sidebar[8 - j] });
    }

    // Inventory
    for (int xx = 0; xx < 3; xx++) {
        float xoff = slef + cmg + xx * (csz + cmg);
        for (int yy = 8; yy >= 0; yy--) {
            float yoff = sbot + cmg + yy * (csz + cmg);
            cells.push_back({ xoff, yoff, xoff + csz, yoff + csz, &inventory[xx * 9 + (8 - yy)] });
        }
    }

    return cells;
}

InventoryLayout::InventoryLayout() {
    cshader = Shader::getShader("flatCShader");
    tshader = Shader::getShader("flatTShader");

    // 6 * 9 * 4 = 216 // vertices in cells
    // 216 + 12 = 228 // vertices in structures

    cbuff = new float[228 * 6];
    tbuff = new float[216 * 2];

    float *collb = cbuff + (228 * 2);
    fillColors1(collb);

    glGenVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
    glGenBuffers(sizeof(vbo) / sizeof(GLuint), vbo);

    // Buff C
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 228 * 6 * SZ_FLT, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 2 * 228 * SZ_FLT, 4 * 228 * SZ_FLT, collb);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(0 * SZ_FLT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * SZ_FLT, (void*)(2 * 228 * SZ_FLT));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Buff T
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 216 * 4 * SZ_FLT, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(0 * SZ_FLT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(2 * 216 * SZ_FLT));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    sidebar = std::vector<Item>(9);
    inventory = std::vector<Item>(27);
}

InventoryLayout::~InventoryLayout() {
    delete[] cbuff;
    delete[] tbuff;
}

size_t InventoryLayout::fillSidebar(float *buff, uint width, uint height) const {
    float cy = height / 2.f;            // Vertical center
    float csz = 50;                     // Sidebar cell size
    float cmg = 7;                      // Sidebar cell margin
    float sw = csz + cmg + cmg;         // Sidebar total width
    float sh = (csz + cmg) * 9 + cmg;   // Sidebar total height
    float sbot = cy - sh / 2.f;         // Sidebar bottom pos

    // Sidebar background
    float vert1[12] = {
        0, sbot,
        sw, sbot,
        sw, sbot + sh,
        0, sbot,
        sw, sbot + sh,
        0, sbot + sh,
    };
    for (int i = 0; i < 12; i++)
        buff[i] = vert1[i];
    buff += 12;

    // Sidebar cells
    float vert3[6][2] = {
        { 0, 0 },
        { csz, 0 },
        { csz, csz },
        { 0, 0 },
        { csz, csz },
        { 0, csz },
    };
    int ind = 0;
    for (int j = 8; j >= 0; j--) {
        float yoff = sbot + cmg + j * (csz + cmg);
        for (int i = 0; i < 6; i++) {
            buff[ind++] = cmg + vert3[i][0];
            buff[ind++] = yoff + vert3[i][1];
        }
    }
    return 120; // 12 + (9 * 6 * 2)
}

size_t InventoryLayout::fillInventory(float *buff, uint width, uint height) const {
    float cy = height / 2.f;            // Vertical center
    float csz = 50;                     // Cell size
    float cmg = 7;                      // Cell margin
    float sw = (csz + cmg) * 3 + cmg;   // Total width
    float sh = (csz + cmg) * 9 + cmg;   // Total height
    float sbot = cy - sh / 2.f;         // Bottom pos
    float slef = csz + cmg + cmg + 20;  // Left pos

    // Background
    float vert1[12] = {
        slef,       sbot,
        slef + sw,  sbot,
        slef + sw,  sbot + sh,
        slef,       sbot,
        slef + sw,  sbot + sh,
        slef,       sbot + sh,
    };
    for (int i = 0; i < 12; i++)
        buff[i] = vert1[i];
    buff += 12;

    // Cells
    float vert3[6][2] = {
        { 0, 0 },
        { csz, 0 },
        { csz, csz },
        { 0, 0 },
        { csz, csz },
        { 0, csz },
    };
    int ind = 0;
    for (int xx = 0; xx < 3; xx++) {
        float xoff = slef + cmg + xx * (csz + cmg);
        for (int yy = 8; yy >= 0; yy--) {
            float yoff = sbot + cmg + yy * (csz + cmg);
            for (int i = 0; i < 6; i++) {
                buff[ind++] = xoff + vert3[i][0];
                buff[ind++] = yoff + vert3[i][1];
            }
        }
    }

    return 336; // 12 + (3 * 9 * 6 * 2)
}

void InventoryLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    size_t offset = 0;
    offset += fillSidebar(cbuff + offset, width, height);
    offset += fillInventory(cbuff + offset, width, height);

    cshader.use();
    cshader.setUniform("projection", m_ortho);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, offset * SZ_FLT, cbuff);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, offset / 2);
    glBindVertexArray(0);


    tshader.use();
    tshader.setUniform("projection", m_ortho);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    auto cells = calcCells(width, height);
    for (size_t i = 0; i < cells.size(); i++) {
        tbuff[i * 12 + 0]  = cells[i].x1;
        tbuff[i * 12 + 1]  = cells[i].y1;
        tbuff[i * 12 + 2]  = cells[i].x2;
        tbuff[i * 12 + 3]  = cells[i].y1;
        tbuff[i * 12 + 4]  = cells[i].x2;
        tbuff[i * 12 + 5]  = cells[i].y2;

        tbuff[i * 12 + 6]  = cells[i].x1;
        tbuff[i * 12 + 7]  = cells[i].y1;
        tbuff[i * 12 + 8]  = cells[i].x2;
        tbuff[i * 12 + 9]  = cells[i].y2;
        tbuff[i * 12 + 10] = cells[i].x1;
        tbuff[i * 12 + 11] = cells[i].y2;
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * 216 * SZ_FLT, tbuff);

    for (size_t i = 0; i < cells.size(); i++) {
        tbuff[i * 12 + 0]  = cells[i].item->tx;
        tbuff[i * 12 + 1]  = cells[i].item->ty;
        tbuff[i * 12 + 2]  = cells[i].item->tx + Item::texSize / static_cast<float>(Item::atlasWidth);
        tbuff[i * 12 + 3]  = cells[i].item->ty;
        tbuff[i * 12 + 4]  = cells[i].item->tx + Item::texSize / static_cast<float>(Item::atlasWidth);
        tbuff[i * 12 + 5]  = cells[i].item->ty + Item::texSize / static_cast<float>(Item::atlasHeight);

        tbuff[i * 12 + 6]  = cells[i].item->tx;
        tbuff[i * 12 + 7]  = cells[i].item->ty;
        tbuff[i * 12 + 8]  = cells[i].item->tx + Item::texSize / static_cast<float>(Item::atlasWidth);
        tbuff[i * 12 + 9]  = cells[i].item->ty + Item::texSize / static_cast<float>(Item::atlasHeight);
        tbuff[i * 12 + 10] = cells[i].item->tx;
        tbuff[i * 12 + 11] = cells[i].item->ty + Item::texSize / static_cast<float>(Item::atlasHeight);
    }
    glBufferSubData(GL_ARRAY_BUFFER, 2 * 216 * SZ_FLT, 2 * 216 * SZ_FLT, tbuff);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    bool isBlockTexure = false;
    glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
    glBindVertexArray(vao[1]);
    for (int i = 0; i < 9; i++) {
        if (sidebar[i].isExist()) {
            if (sidebar[i].isBlock() && !isBlockTexure) {
                isBlockTexure = true;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("blocks"));
            }
            else if (sidebar[i].isItem() && isBlockTexure) {
                isBlockTexure = false;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
            }
            glDrawArrays(GL_TRIANGLES, i * 6, 6);
        }
    }
    for (int i = 0; i < 27; i++) {
        if (inventory[i].isExist()) {
            if (inventory[i].isBlock() && !isBlockTexure) {
                isBlockTexure = true;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("blocks"));
            }
            else if (inventory[i].isItem() && isBlockTexure) {
                isBlockTexure = false;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
            }
            glDrawArrays(GL_TRIANGLES, 9 * 6 + i * 6, 6);
        }
    }

    glBindVertexArray(0);
}

void InventoryLayout::updateSidebar(const std::vector<Item> &items) {
    this->sidebar = items;
}

void InventoryLayout::updateInventory(const std::vector<Item> &items) {
    this->inventory = items;
}
