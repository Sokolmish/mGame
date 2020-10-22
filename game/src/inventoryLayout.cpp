#include "../include/inventoryLayout.hpp"
#include "../include/util/image.hpp"

#define SZ_FLT sizeof(GLfloat)

InventoryLayout::InventoryLayout() {
    cshader = Shader::getShader("flatCShader");
    tshader = Shader::getShader("flatTShader");

    // 6 * 9 * 4 = 216 // vertices in cells
    // 216 + 12 = 228 // vertices in structures

    cbuff = new GLfloat[228 * 2];
    tbuff = new GLfloat[216 * 2];

    glGenVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
    glGenBuffers(sizeof(vbo) / sizeof(GLuint), vbo);

    // Buff C
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 228 * 6 * SZ_FLT, nullptr, GL_DYNAMIC_DRAW);
    // Fill colors
    GLfloat *collb = new float[228 * 4];

    for (uint i = 0; i < 6; i++) // Sidebar background
        for (uint j = 0; j < 4; j++)
            collb[(0 + i) * 4 + j] = cfg.inv_back_color[j];
    for (uint i = 0; i < 54; i++) // Sidebar cells 54 = 6 * 9
        for (uint j = 0; j < 4; j++)
            collb[(6 + i) * 4 + j] = cfg.inv_cell_color[j];
    for (uint i = 0; i < 6; i++) // Inventory background
        for (uint j = 0; j < 4; j++)
            collb[(60 + i) * 4 + j] = cfg.inv_back_color[j];
    for (uint i = 0; i < 162; i++) // Inventory cells 162 = 6 * 9 * 3
        for (uint j = 0; j < 4; j++)
            collb[(66 + i) * 4 + j] = cfg.inv_cell_color[j];

    glBufferSubData(GL_ARRAY_BUFFER, 2 * 228 * SZ_FLT, 4 * 228 * SZ_FLT, collb);
    delete collb;

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

std::vector<InventoryLayout::Cell> InventoryLayout::calcCells(uint width, uint height) const {
    std::vector<InventoryLayout::Cell> cells = std::vector<InventoryLayout::Cell>(0);

    for (int j = 8; j >= 0; j--) {
        float yoff = cfg.int_bot_pos + cfg.cell_margin + j * (cfg.cell_size + cfg.cell_margin);
        cells.push_back({
            cfg.cell_margin, yoff,
            cfg.cell_margin + cfg.cell_size, yoff + cfg.cell_size,
            &sidebar[8 - j]
        });
    }

    // Inventory
    for (int xx = 0; xx < 3; xx++) {
        float xoff = cfg.inv_left_pos + cfg.cell_margin + xx * (cfg.cell_size + cfg.cell_margin);
        for (int yy = 8; yy >= 0; yy--) {
            float yoff = cfg.int_bot_pos + cfg.cell_margin + yy * (cfg.cell_size + cfg.cell_margin);
            cells.push_back({
                xoff, yoff, xoff + cfg.cell_size,
                yoff + cfg.cell_size,
                &inventory[xx * 9 + (8 - yy)]
            });
        }
    }

    return cells;
}

size_t InventoryLayout::fillSidebar(float *buff, uint width, uint height) const {
    // Sidebar background
    float vert1[12] = {
        0,              cfg.int_bot_pos,
        cfg.side_width, cfg.int_bot_pos,
        cfg.side_width, cfg.int_bot_pos + cfg.int_height,
        0,              cfg.int_bot_pos,
        cfg.side_width, cfg.int_bot_pos + cfg.int_height,
        0,              cfg.int_bot_pos + cfg.int_height,
    };
    for (int i = 0; i < 12; i++)
        buff[i] = vert1[i];
    buff += 12;

    // Sidebar cells
    float vert3[6][2] = {
        { 0, 0 },
        { cfg.cell_size, 0 },
        { cfg.cell_size, cfg.cell_size },
        { 0, 0 },
        { cfg.cell_size, cfg.cell_size },
        { 0, cfg.cell_size },
    };
    int ind = 0;
    for (int j = 8; j >= 0; j--) {
        float yoff = cfg.int_bot_pos + cfg.cell_margin + j * (cfg.cell_size + cfg.cell_margin);
        for (int i = 0; i < 6; i++) {
            buff[ind++] = cfg.cell_margin + vert3[i][0];
            buff[ind++] = yoff + vert3[i][1];
        }
    }
    return 120; // 12 + (9 * 6 * 2)
}

size_t InventoryLayout::fillInventory(float *buff, uint width, uint height) const {
    // Background
    float vert1[12] = {
        cfg.inv_left_pos,                 cfg.int_bot_pos,
        cfg.inv_left_pos + cfg.inv_width, cfg.int_bot_pos,
        cfg.inv_left_pos + cfg.inv_width, cfg.int_bot_pos + cfg.int_height,
        cfg.inv_left_pos,                 cfg.int_bot_pos,
        cfg.inv_left_pos + cfg.inv_width, cfg.int_bot_pos + cfg.int_height,
        cfg.inv_left_pos,                 cfg.int_bot_pos + cfg.int_height,
    };
    for (int i = 0; i < 12; i++)
        buff[i] = vert1[i];
    buff += 12;

    // Cells
    float vert3[6][2] = {
        { 0, 0 },
        { cfg.cell_size, 0 },
        { cfg.cell_size, cfg.cell_size },
        { 0, 0 },
        { cfg.cell_size, cfg.cell_size },
        { 0, cfg.cell_size },
    };
    int ind = 0;
    for (int xx = 0; xx < 3; xx++) {
        float xoff = cfg.inv_left_pos + cfg.cell_margin + xx * (cfg.cell_size + cfg.cell_margin);
        for (int yy = 8; yy >= 0; yy--) {
            float yoff = cfg.int_bot_pos + cfg.cell_margin + yy * (cfg.cell_size + cfg.cell_margin);
            for (int i = 0; i < 6; i++) {
                buff[ind++] = xoff + vert3[i][0];
                buff[ind++] = yoff + vert3[i][1];
            }
        }
    }

    return 336; // 12 + (3 * 9 * 6 * 2)
}

void InventoryLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
    cfg = iventory_config::Config(width, height);

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
        tbuff[i * 12 + 1]  = cells[i].item->ty + Item::texSize / static_cast<float>(Item::atlasHeight);
        tbuff[i * 12 + 2]  = cells[i].item->tx + Item::texSize / static_cast<float>(Item::atlasWidth);
        tbuff[i * 12 + 3]  = cells[i].item->ty + Item::texSize / static_cast<float>(Item::atlasHeight);
        tbuff[i * 12 + 4]  = cells[i].item->tx + Item::texSize / static_cast<float>(Item::atlasWidth);
        tbuff[i * 12 + 5]  = cells[i].item->ty;

        tbuff[i * 12 + 6]  = cells[i].item->tx;
        tbuff[i * 12 + 7]  = cells[i].item->ty + Item::texSize / static_cast<float>(Item::atlasHeight);
        tbuff[i * 12 + 8]  = cells[i].item->tx + Item::texSize / static_cast<float>(Item::atlasWidth);
        tbuff[i * 12 + 9]  = cells[i].item->ty;
        tbuff[i * 12 + 10] = cells[i].item->tx;
        tbuff[i * 12 + 11] = cells[i].item->ty;
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
