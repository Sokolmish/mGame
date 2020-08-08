#include "../include/interfaceLayout.hpp"
#include "../include/util/image.hpp"

#define SZ_FLT sizeof(float)

void fillColors(float *collb) {
    for (uint i = 0; i < 12; i++) { // Crosshair
        collb[i * 4 + 0] = 1.0f;
        collb[i * 4 + 1] = 1.0f;
        collb[i * 4 + 2] = 1.0f;
        collb[i * 4 + 3] = 1.0f;
    }
    for (uint i = 0; i < 6; i++) { // Sidebar background
        collb[(12 + i) * 4 + 0] = 0.6f;
        collb[(12 + i) * 4 + 1] = 0.6f;
        collb[(12 + i) * 4 + 2] = 0.6f;
        collb[(12 + i) * 4 + 3] = 1.0f;
    }
    for (uint i = 0; i < 6; i++) { // Sidebar selection
        collb[(18 + i) * 4 + 0] = 0.85f;
        collb[(18 + i) * 4 + 1] = 0.85f;
        collb[(18 + i) * 4 + 2] = 0.85f;
        collb[(18 + i) * 4 + 3] = 1.0f;
    }
    for (uint i = 0; i < 54; i++) { // Sidebar cells
        collb[(24 + i) * 4 + 0] = 0.35f;
        collb[(24 + i) * 4 + 1] = 0.35f;
        collb[(24 + i) * 4 + 2] = 0.35f;
        collb[(24 + i) * 4 + 3] = 1.0f;
    }
}

InterfaceLayout::InterfaceLayout() {
    cshader = Shader::getShader("flatCShader");
    tshader = Shader::getShader("flatTShader");

    // Crosshair - 12
    // Sidebar - 6
    // Sidebar selection - 6
    // Sidebar cells - 54
    vertexCount = 78;

    buff = new float[vertexCount * 6]; // cbuff
    float *collb = buff + (vertexCount * 2);
    fillColors(collb);

    glGenVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
    glGenBuffers(sizeof(vbo) / sizeof(GLuint), vbo);

    // Buff C
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 6 * SZ_FLT, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 2 * vertexCount * SZ_FLT, 4 * vertexCount * SZ_FLT, collb);

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
    float cx = width / 2.f;     // Horizontal center
    float cy = height / 2.f;    // Vertical center
    float crw = 2.f;            // Crosshair width
    float cro = 10.f;           // Crosshair offset
    float vertices[24] = {
        cx - crw, cy + cro,
        cx - crw, cy - cro,
        cx + crw, cy - cro,
        cx - crw, cy + cro,
        cx + crw, cy - cro,
        cx + crw, cy + cro,
        cx - cro, cy + crw,
        cx - cro, cy - crw,
        cx + cro, cy - crw,
        cx - cro, cy + crw,
        cx + cro, cy - crw,
        cx + cro, cy + crw,
    };
    for (int i = 0; i < 24; i++)
        buff[i] = vertices[i];
    return 24;
}

size_t InterfaceLayout::fillSidebar(float *buff, uint width, uint height) const {
    float cy = height / 2.f;            // Vertical center
    float csz = 50;                     // Sidebar cell size
    float cmg = 7;                      // Sidebar cell margin
    float sw = csz + cmg + cmg;         // Sidebar total width
    float sh = (csz + cmg) * 9 + cmg;   // Sidebar total height
    float sbot = cy - sh / 2.f;         // Sidebar bottom pos
    // Sidebar
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

    // Selection
    int tselected = 8 - selectedCell;
    float ssz = cmg + csz + cmg;
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
        float yoff = sbot + tselected * (cmg + csz);
        buff[ind++] = vert2[i][0];
        buff[ind++] = yoff + vert2[i][1];
    }
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
    ind = 0;
    for (int j = 8; j >= 0; j--) {
        float yoff = sbot + cmg + j * (csz + cmg);
        for (int i = 0; i < 6; i++) {
            buff[ind++] = cmg + vert3[i][0];
            buff[ind++] = yoff + vert3[i][1];
        }
    }
    return 132;
}

void InterfaceLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
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
            tbuff[ind++] = sidebar[i].ty;
            tbuff[ind++] = sidebar[i].tx + Item::texSize / static_cast<float>(Item::atlasWidth);
            tbuff[ind++] = sidebar[i].ty;
            tbuff[ind++] = sidebar[i].tx + Item::texSize / static_cast<float>(Item::atlasWidth);
            tbuff[ind++] = sidebar[i].ty + Item::texSize / static_cast<float>(Item::atlasHeight);
            tbuff[ind++] = sidebar[i].tx;
            tbuff[ind++] = sidebar[i].ty;
            tbuff[ind++] = sidebar[i].tx + Item::texSize / static_cast<float>(Item::atlasWidth);
            tbuff[ind++] = sidebar[i].ty + Item::texSize / static_cast<float>(Item::atlasHeight);
            tbuff[ind++] = sidebar[i].tx;
            tbuff[ind++] = sidebar[i].ty + Item::texSize / static_cast<float>(Item::atlasHeight);
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