#include "../include/interface.hpp"

//
// GameOverlay
//

GameOverlay::GameOverlay() {
    // Crosshair - 12
    // Sidebar - 6
    // Sidebar selection - 6
    // Sidebar cells - 54
    vertexCount = 78;

    buff = new float[vertexCount * 6];
    float *collb = buff + (vertexCount * 2);
    // Crosshair
    for (uint i = 0; i < 12; i++) {
        collb[i * 4 + 0] = 1.0f;
        collb[i * 4 + 1] = 1.0f;
        collb[i * 4 + 2] = 1.0f;
        collb[i * 4 + 3] = 1.0f;
    }
    for (uint i = 0; i < 6; i++) {
        collb[(12 + i) * 4 + 0] = 0.6f;
        collb[(12 + i) * 4 + 1] = 0.6f;
        collb[(12 + i) * 4 + 2] = 0.6f;
        collb[(12 + i) * 4 + 3] = 1.0f;
    }
    for (uint i = 0; i < 6; i++) {
        collb[(18 + i) * 4 + 0] = 0.85f;
        collb[(18 + i) * 4 + 1] = 0.85f;
        collb[(18 + i) * 4 + 2] = 0.85f;
        collb[(18 + i) * 4 + 3] = 1.0f;
    }
    for (uint i = 0; i < 54; i++) {
        collb[(24 + i) * 4 + 0] = 0.35f;
        collb[(24 + i) * 4 + 1] = 0.35f;
        collb[(24 + i) * 4 + 2] = 0.35f;
        collb[(24 + i) * 4 + 3] = 1.0f;
    }

    selectedCell = 1;
}

GameOverlay::~GameOverlay() {
    delete[] buff;
}

size_t GameOverlay::getReserveBuffSize() const {
    return vertexCount * 6 * sizeof(float);
}

size_t GameOverlay::getVertexCount() const {
    return vertexCount;
}

size_t GameOverlay::fillCrosshair(float *buff, uint width, uint height) const {
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

size_t GameOverlay::fillSidebar(float *buff, uint width, uint height) const {
    // float cx = width / 2.f;             // Horizontal center
    float cy = height / 2.f;            // Vertical center
    float csz = 40;                     // Sidebar cell size
    float cmg = 6;                      // Sidebar cell margin
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
        float yoff = sbot + selectedCell * (cmg + csz);
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
    for (int j = 0; j < 9; j++) {
        float yoff = sbot + cmg + j * (csz + cmg);
        for (int i = 0; i < 6; i++) {
            buff[ind++] = cmg + vert3[i][0];
            buff[ind++] = yoff + vert3[i][1];
        }
    }
    return 132;
}

float* GameOverlay::getLocBuff(uint width, uint height) const {
    size_t offset = 0;
    offset += fillCrosshair(buff + offset, width, height);    
    offset += fillSidebar(buff + offset, width, height);
    return buff;
}

float* GameOverlay::getColBuff() const {
    return buff + (vertexCount * 2);
}

void GameOverlay::selectSidebarCell(int cellnum) {
    selectedCell = cellnum;
}