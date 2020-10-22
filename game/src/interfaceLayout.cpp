#include "../include/interfaceLayout.hpp"
#include "../include/util/image.hpp"

#define SZ_FLT sizeof(float)

InterfaceLayout::InterfaceLayout() {
    cshader = Shader::getShader("flatCShader");
    tshader = Shader::getShader("flatTShader");

    // Crosshair - 12
    // Sidebar - 6
    // Sidebar selection - 6
    // Sidebar cells - 54
    vertexCount = 78;

    buff = new float[vertexCount * 6]; // cbuff

    glGenVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
    glGenBuffers(sizeof(vbo) / sizeof(GLuint), vbo);

    // Buff C
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 6 * SZ_FLT, nullptr, GL_DYNAMIC_DRAW);
    // Fill colors
    GLfloat *collb = new float[4 * vertexCount];
    for (uint i = 0; i < 12; i++) // Crosshair
        for (uint j = 0; j < 4; j++)
            collb[(i + 0) * 4 + j] = cfg.crosshair_color[j];
    for (uint i = 0; i < 6; i++) // Sidebar background
        for (uint j = 0; j < 4; j++)
            collb[(i + 12) * 4 + j] = cfg.inv_back_color[j];
    for (uint i = 0; i < 6; i++) // Sidebar selection
        for (uint j = 0; j < 4; j++)
            collb[(i + 18) * 4 + j] = cfg.inv_select_color[j];
    for (uint i = 0; i < 54; i++) // Sidebar cells
        for (uint j = 0; j < 4; j++)
            collb[(i + 24) * 4 + j] = cfg.inv_cell_color[j];
    glBufferSubData(GL_ARRAY_BUFFER, 2 * vertexCount * SZ_FLT, 4 * vertexCount * SZ_FLT, collb);
    delete[] collb;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(0 * SZ_FLT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * SZ_FLT, (void*)(2 * vertexCount * SZ_FLT));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Buff T
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 54 * 4 * SZ_FLT, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(0 * SZ_FLT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(2 * 54 * SZ_FLT));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Other
    selectedCell = 1;
}

InterfaceLayout::~InterfaceLayout() {
    delete[] buff;
}

size_t InterfaceLayout::fillCrosshair(float *buff, uint width, uint height) const {
    float vertices[24] = {
        cfg.hor_center - cfg.cross_width, cfg.vert_center + cfg.cross_offse,
        cfg.hor_center - cfg.cross_width, cfg.vert_center - cfg.cross_offse,
        cfg.hor_center + cfg.cross_width, cfg.vert_center - cfg.cross_offse,
        cfg.hor_center - cfg.cross_width, cfg.vert_center + cfg.cross_offse,
        cfg.hor_center + cfg.cross_width, cfg.vert_center - cfg.cross_offse,
        cfg.hor_center + cfg.cross_width, cfg.vert_center + cfg.cross_offse,
        cfg.hor_center - cfg.cross_offse, cfg.vert_center + cfg.cross_width,
        cfg.hor_center - cfg.cross_offse, cfg.vert_center - cfg.cross_width,
        cfg.hor_center + cfg.cross_offse, cfg.vert_center - cfg.cross_width,
        cfg.hor_center - cfg.cross_offse, cfg.vert_center + cfg.cross_width,
        cfg.hor_center + cfg.cross_offse, cfg.vert_center - cfg.cross_width,
        cfg.hor_center + cfg.cross_offse, cfg.vert_center + cfg.cross_width,
    };
    for (int i = 0; i < 24; i++)
        buff[i] = vertices[i];
    return 24;
}

size_t InterfaceLayout::fillSidebar(float *buff, uint width, uint height) const {
    // Sidebar
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

    // Selection
    int tselected = 8 - selectedCell;
    float ssz = cfg.cell_margin + cfg.cell_size + cfg.cell_margin;
    float vert2[6][2] = {
        { 0, 0 },
        { ssz, 0 },
        { ssz, ssz },
        { 0, 0 },
        { ssz, ssz },
        { 0, ssz },
    };
    int ind = 0;
    for (int i = 0; i < 6; i++) {
        float yoff = cfg.int_bot_pos + tselected * (cfg.cell_margin + cfg.cell_size);
        buff[ind++] = vert2[i][0];
        buff[ind++] = yoff + vert2[i][1];
    }
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
    ind = 0;
    for (int j = 8; j >= 0; j--) {
        float yoff = cfg.int_bot_pos + cfg.cell_margin + j * (cfg.cell_size + cfg.cell_margin);
        for (int i = 0; i < 6; i++) {
            buff[ind++] = cfg.cell_margin + vert3[i][0];
            buff[ind++] = yoff + vert3[i][1];
        }
    }
    return 132;
}

void InterfaceLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
    cfg = iventory_config::Config(width, height);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    size_t offset = 0;
    offset += fillCrosshair(buff + offset, width, height);
    offset += fillSidebar(buff + offset, width, height);

    cshader.use();
    cshader.setUniform("projection", m_ortho);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 2 * SZ_FLT, buff);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);

    float tbuff[2 * 54];
    uint ind;
    for (uint i = 0; i < 9; i++) {
        if (sidebar[i].Id != 0) {
            tbuff[ind++] = sidebar[i].tx;
            tbuff[ind++] = sidebar[i].ty + Item::texSize / static_cast<float>(Item::atlasHeight);
            tbuff[ind++] = sidebar[i].tx + Item::texSize / static_cast<float>(Item::atlasWidth);
            tbuff[ind++] = sidebar[i].ty + Item::texSize / static_cast<float>(Item::atlasHeight);
            tbuff[ind++] = sidebar[i].tx + Item::texSize / static_cast<float>(Item::atlasWidth);
            tbuff[ind++] = sidebar[i].ty;
            tbuff[ind++] = sidebar[i].tx;
            tbuff[ind++] = sidebar[i].ty + Item::texSize / static_cast<float>(Item::atlasHeight);
            tbuff[ind++] = sidebar[i].tx + Item::texSize / static_cast<float>(Item::atlasWidth);
            tbuff[ind++] = sidebar[i].ty;
            tbuff[ind++] = sidebar[i].tx;
            tbuff[ind++] = sidebar[i].ty;
        }
        else {
            ind += 12;
        }
    }

    tshader.use();
    tshader.setUniform("projection", m_ortho);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 54 * 2 * SZ_FLT, buff + 48);
    glBufferSubData(GL_ARRAY_BUFFER, 54 * 2 * SZ_FLT, 54 * 2 * SZ_FLT, tbuff);
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
    glBindVertexArray(0);
}

void InterfaceLayout::selectSidebarCell(int cellnum) {
    selectedCell = cellnum;
}
void InterfaceLayout::updateSidebarItems(const std::vector<Item> &items) {
    sidebar = items;
}