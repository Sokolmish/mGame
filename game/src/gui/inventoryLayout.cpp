#include "gui/inventoryLayout.hpp"
#include "util/image.hpp"

using namespace std::string_literals;

#define SZ_FLT sizeof(GLfloat)

static void loadRectange(float *buff, float ax, float ay, float bx, float by) {
    float vertices[] {
        ax, ay,     bx, ay,     bx, by,
        ax, ay,     bx, by,     ax, by
    };
    for (size_t i = 0; i < sizeof(vertices) / sizeof(float); i++)
        buff[i] = vertices[i];
}

InventoryLayout::InventoryLayout(Player *player) {
    tshader = Shader::getShader("flatTShader");

    vertexCount = (1 + 1 + 9 + 27) * 6;
    buff = new GLfloat[vertexCount * 4];
    for (int i = 0; i < vertexCount * 4; i++)
        buff[i] = 0.f;

    size_t offset = vertexCount * 2;
    const float texWidth = 380;
    const float texHeight = 620;

    // Sidebar tex
    loadRectange(buff + offset,
        0 / texWidth,   0 / texHeight,
        76 / texWidth,  620 / texHeight
    );
    offset += 12;
    // Inventory tex
    loadRectange(buff + offset,
        152 / texWidth, 0 / texHeight,
        364 / texWidth, 620 / texHeight
    );
    offset += 12;

    glGenVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
    glGenBuffers(sizeof(vbo) / sizeof(GLuint), vbo);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 4 * SZ_FLT, buff, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(0 * SZ_FLT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * SZ_FLT, (void*)(2 * vertexCount * SZ_FLT));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->player = player;
    cells = std::vector<Cell>();
}

InventoryLayout::~InventoryLayout() {
    delete[] buff;
}

void InventoryLayout::changeGeometry(float width, float height) {
    static_cast<void>(width);
    
    cells = std::vector<Cell>();

    size_t offset = 0;
    float vert_center = height / 2.f;
    float bot_pos = vert_center - cfg.int_height / 2.f;

    // Sidebar pos
    loadRectange(buff + offset,
        0.f, bot_pos,
        cfg.side_width, bot_pos + cfg.int_height
    );
    offset += 12;
    // Inventory pos
    loadRectange(buff + offset,
        cfg.inv_left_pos, bot_pos,
        cfg.inv_left_pos + cfg.inv_width, bot_pos + cfg.int_height
    );
    offset += 12;
    // Sidebar items pos
    for (int j = 8; j >= 0; j--) {
        const Item &cellItem = player->sidebar[8 - j];
        float yoff = bot_pos + cfg.cell_margin + j * (cfg.cell_size + cfg.cell_margin);
        if (cellItem.isExist()) {
            loadRectange(buff + offset,
                cfg.cell_margin, yoff,
                cfg.cell_margin + cfg.cell_size, yoff + cfg.cell_size
            );
        }
        cells.push_back(Cell{
            cfg.cell_margin, yoff,
            cfg.cell_margin + cfg.cell_size, yoff + cfg.cell_size,
            true, j
        });
        offset += 12;
    }
    // Inventory items pos
    for (int xx = 0; xx < 3; xx++) {
        float xoff = cfg.inv_left_pos + cfg.cell_margin + xx * (cfg.cell_size + cfg.cell_margin);
        for (int yy = 8; yy >= 0; yy--) {
            const Item &cellItem = player->inventory[xx * 9 + (8 - yy)];
            float yoff = bot_pos + cfg.cell_margin + yy * (cfg.cell_size + cfg.cell_margin);
            if (cellItem.isExist()) {
                loadRectange(buff + offset,
                    xoff, yoff,
                    xoff + cfg.cell_size, yoff + cfg.cell_size
                );
            }
            cells.push_back(Cell{
                xoff, yoff,
                xoff + cfg.cell_size, yoff + cfg.cell_size,
                false, xx * 9 + yy
            });
            offset += 12;
        }
    }
}

void InventoryLayout::show(const glm::mat4 &m_ortho) {
    size_t offset = (vertexCount * 2 + 24);
    float itemWidth = Item::texSize / static_cast<float>(Item::atlasWidth);
    float itemHeight = Item::texSize / static_cast<float>(Item::atlasHeight);

    // Items texs
    for (int j = 8; j >= 0; j--) {
        const Item &cellItem = player->sidebar[8 - j];
        if (cellItem.isExist()) {
            loadRectange(buff + offset,
                cellItem.tx, cellItem.ty + itemHeight,
                cellItem.tx + itemWidth, cellItem.ty
            );
        }
        offset += 12;
    }
    for (int xx = 0; xx < 3; xx++) {
        for (int yy = 8; yy >= 0; yy--) {
            const Item &cellItem = player->inventory[xx * 9 + (8 - yy)];
            if (cellItem.isExist()) {
                loadRectange(buff + offset,
                    cellItem.tx, cellItem.ty + itemHeight,
                    cellItem.tx + itemWidth, cellItem.ty
                );
            }
            offset += 12;
        }
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tshader.use();
    tshader.setUniform("projection"s, m_ortho);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 4 * SZ_FLT, buff, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao[0]);

    glBindTexture(GL_TEXTURE_2D, Image::loadImage("interface"));
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
    bool isBlockTexure = false;
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
            glDrawArrays(GL_TRIANGLES, 12 + i * 6, 6);
        }
    }
    for (int i = 0; i < 27; i++) {
        if (player->inventory[i].isExist()) {
            if (player->inventory[i].isBlock() && !isBlockTexure) {
                isBlockTexure = true;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("blocks"));
            }
            else if (player->inventory[i].isItem() && isBlockTexure) {
                isBlockTexure = false;
                glBindTexture(GL_TEXTURE_2D, Image::loadImage("items"));
            }
            glDrawArrays(GL_TRIANGLES, 66 + i * 6, 6);
        }
    }

    glBindVertexArray(0);
}

void InventoryLayout::click(float x, float y, int code, int action) {
    if (code == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        for (const auto &cell : cells) {
            if (cell.x1 <= x && x <= cell.x2 && cell.y1 <= y && y <= cell.y2) {
                Item &curItem = cell.isSidebar ?
                    player->sidebar[cell.index] :
                    player->inventory[cell.index];

                if (curItem.isExist() && pickedItem.isExist()) {
                    std::swap(pickedItem, curItem);
                }
                else if (curItem.isExist()) {
                    pickedItem = curItem;
                    curItem = Item();
                }
                else if (pickedItem.isExist()) {
                    curItem = pickedItem;
                    pickedItem = Item();
                }

                break;
            }
        }
    }
}
