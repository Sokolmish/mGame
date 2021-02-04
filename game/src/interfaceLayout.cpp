#include "../include/interfaceLayout.hpp"
#include "../include/util/image.hpp"

#define SZ_FLT sizeof(float)

static void loadRectange(float *buff, float ax, float ay, float bx, float by) {
    float vertices[12] = {
        ax, ay,     bx, ay,     bx, by,
        ax, ay,     bx, by,     ax, by
    };
    for (int i = 0; i < 12; i++)
        buff[i] = vertices[i];
}

InterfaceLayout::InterfaceLayout() {
    tshader = Shader::getShader("flatTShader");

    // Cross - 6
    // Side - 6
    // Select - 6
    // Cells - 9 * 6 = 54
    vertexCount = 72;
    buff = new float[vertexCount * 4];
    for (int i = 0; i < vertexCount * 4; i++)
        buff[i] = 0.f;
    size_t offset = vertexCount * 2;
    loadRectange(buff + offset, .4222f, .8452f, .5333f, .8774f);
    offset += 12;
    loadRectange(buff + offset, .0f, .0f, .4222f, 1.f);
    offset += 12;
    loadRectange(buff + offset, .4222f, .8774f, .8444f, 1.f);
    offset += 12;

    glGenVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
    glGenBuffers(sizeof(vbo) / sizeof(GLuint), vbo);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 4 * SZ_FLT, buff, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(0 * SZ_FLT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(2 * vertexCount * SZ_FLT));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Other
    selectedCell = 1;
}

InterfaceLayout::~InterfaceLayout() {
    delete[] buff;
}

void InterfaceLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
    cfg = iventory_config::Config(width, height);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    size_t offset = 0;

    // Crosshair
    loadRectange(buff + offset,
        cfg.hor_center - cfg.cross_wsize, cfg.vert_center - cfg.cross_hsize,
        cfg.hor_center + cfg.cross_wsize, cfg.vert_center + cfg.cross_hsize
    );
    offset += 12;
    // Sidebar
    loadRectange(buff + offset,
        0, cfg.int_bot_pos,
        cfg.side_width, cfg.int_bot_pos + cfg.int_height
    );
    offset += 12;
    // Selection
    float selYOff = cfg.int_bot_pos + (8 - selectedCell) * (cfg.cell_margin + cfg.cell_size);
    float selSize = cfg.cell_margin + cfg.cell_size + cfg.cell_margin;
    loadRectange(buff + offset, 0, selYOff, selSize, selYOff + selSize);
    offset += 12;
    // Items
    for (int j = 8; j >= 0; j--) {
        float itemYOff = cfg.int_bot_pos + cfg.cell_margin + j * (cfg.cell_size + cfg.cell_margin);
        loadRectange(buff + offset,
            cfg.cell_margin, itemYOff,
            cfg.cell_margin + cfg.cell_size, itemYOff + cfg.cell_size
        );
        offset += 12;
    }

    // Fill items textures
    float tbuff[2 * 54];
    float itemWidth = Item::texSize / static_cast<float>(Item::atlasWidth);
    float itemHeight = Item::texSize / static_cast<float>(Item::atlasHeight);
    for (uint i = 0; i < 9; i++) {
        if (sidebar[i].Id != 0) {
            loadRectange(tbuff + i * 12,
                sidebar[i].tx, sidebar[i].ty + itemHeight,
                sidebar[i].tx + itemWidth, sidebar[i].ty
            );
        }
    }

    uint itemsTexOffset = (vertexCount * 2 + 36) * SZ_FLT;

    tshader.use();
    tshader.setUniform("projection", m_ortho);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 2 * SZ_FLT, buff);      // Coordinates
    glBufferSubData(GL_ARRAY_BUFFER, itemsTexOffset, 54 * 2 * SZ_FLT, tbuff);  // Texture
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao[0]);
    glBindTexture(GL_TEXTURE_2D, Image::loadImage("interface"));
    glDrawArrays(GL_TRIANGLES, 0, 18);

    bool isBlockTexure = false;
    glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
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
            glDrawArrays(GL_TRIANGLES, 18 + i * 6, 6); // TODO: QUADS?
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
