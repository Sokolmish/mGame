#include "gui/interfaceLayout.hpp"
#include "util/image.hpp"

#define SZ_FLT sizeof(float)

static void loadRectange(float *buff, float ax, float ay, float bx, float by) {
    float vertices[] {
        ax, ay,     bx, ay,     bx, by,
        ax, ay,     bx, by,     ax, by
    };
    for (size_t i = 0; i < sizeof(vertices) / sizeof(float); i++)
        buff[i] = vertices[i];
}

InterfaceLayout::InterfaceLayout(Player *player) {
    tshader = Shader::getShader("flatTShader");

    vertexCount = 6 + 6 + 6 + (9 * 6); // 72
    buff = new float[vertexCount * 4];
    for (int i = 0; i < vertexCount * 4; i++)
        buff[i] = 0.f;

    size_t offset = vertexCount * 2;
    const float texWidth = 380;
    const float texHeight = 620;

    // Cross tex
    loadRectange(buff + offset,
        76 / texWidth,  524 / texHeight,
        96 / texWidth,  544 / texHeight
    );
    offset += 12;
    // Sidebar tex
    loadRectange(buff + offset,
        0 / texWidth,   0 / texHeight,
        76 / texWidth,  620 / texHeight
    );
    offset += 12;
    // Selection tex
    loadRectange(buff + offset,
        76 / texWidth,  544 / texHeight,
        152 / texWidth, 620 / texHeight
    );
    offset += 12;

    glGenVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
    glGenBuffers(sizeof(vbo) / sizeof(GLuint), vbo);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 4 * SZ_FLT, buff, GL_STREAM_DRAW);
    // TODO: refactor for DYNAMIC_DRAW
    // TODO: elements buffer

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(0 * SZ_FLT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(2 * vertexCount * SZ_FLT));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Other
    this->player = player;
    this->selectedCell = 1;
}

InterfaceLayout::~InterfaceLayout() {
    delete[] buff;
}

void InterfaceLayout::changeGeometry(float width, float height) {
    size_t offset = 0;

    float hor_center = width / 2.f;
    float vert_center = height / 2.f;
    float bot_pos = vert_center - cfg.int_height / 2.f;

    // Crosshair pos
    loadRectange(buff + offset,
        hor_center - cfg.cross_wsize, vert_center - cfg.cross_hsize,
        hor_center + cfg.cross_wsize, vert_center + cfg.cross_hsize
    );
    offset += 12;
    // Sidebar pos
    loadRectange(buff + offset, 0.f, bot_pos, cfg.side_width, bot_pos + cfg.int_height);
    offset += 12;
    // Selection pos
    float selYOff = bot_pos + (8 - selectedCell) * (cfg.cell_margin + cfg.cell_size);
    float selSize = cfg.cell_margin + cfg.cell_size + cfg.cell_margin;
    loadRectange(buff + offset, 0, selYOff, selSize, selYOff + selSize);
    offset += 12;
    // Items pos
    for (int j = 8; j >= 0; j--) {
        float itemYOff = bot_pos + cfg.cell_margin + j * (cfg.cell_size + cfg.cell_margin);
        loadRectange(buff + offset,
            cfg.cell_margin, itemYOff,
            cfg.cell_margin + cfg.cell_size, itemYOff + cfg.cell_size
        );
        offset += 12;
    }
}

void InterfaceLayout::show(const glm::mat4 &m_ortho) {
    // Items tex
    size_t offset = (vertexCount * 2 + 36);
    float itemWidth = Item::texSize / static_cast<float>(Item::atlasWidth);
    float itemHeight = Item::texSize / static_cast<float>(Item::atlasHeight);
    for (uint i = 0; i < 9; i++) {
        const Item &cellItem = player->sidebar[i];
        if (cellItem.isExist()) {
            loadRectange(buff + offset,
                cellItem.tx, cellItem.ty + itemHeight,
                cellItem.tx + itemWidth, cellItem.ty
            );
        }
        offset += 12;
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tshader.use();
    tshader.setUniform("projection", m_ortho);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 4 * SZ_FLT, buff, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao[0]);

    glBindTexture(GL_TEXTURE_2D, Image::loadImage("interface"));
    glDrawArrays(GL_TRIANGLES, 0, 18);

    bool isBlockTexure = false;
    glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
    for (int i = 0; i < 9; i++) {
        if (player->sidebar[i].isExist()) {
            if (player->sidebar[i].isBlock() && !isBlockTexure) {
                isBlockTexure = true;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("blocks"));
            }
            else if (player->sidebar[i].isItem() && isBlockTexure) {
                isBlockTexure = false;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
            }
            glDrawArrays(GL_TRIANGLES, 18 + i * 6, 6);
        }
    }
    glBindVertexArray(0);
}

void InterfaceLayout::selectSidebarCell(int cellnum) {
    selectedCell = cellnum;
}

void InterfaceLayout::updateSidebarItems(const std::vector<Item> &items) {
    player->sidebar = items;
}
