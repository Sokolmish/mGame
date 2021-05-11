#include "block.hpp"

// Should be redefined in main!
uint Block::atlasWidth = 0;   // 128
uint Block::atlasHeight = 0;  // 128
uint Block::texSize = 0;      // 16
std::map<uint16_t, Block> Block::blocksLib;

Block::Block() {
    id = 0;
}

Block::Block(uint16_t id) {
    *this = blocksLib[id];
}

Item Block::toItem() const {
    return Item(id);
}

uint16_t Block::getId() const {
    return id;
}

glm::vec2 Block::getTex() const {
    return tex;
}

Block Block::createBlock(uint16_t id, const glm::vec2 &tex) {
    Block res;
    res.id = id;
    res.tex.x = tex.x * texSize / (float)atlasWidth;
    res.tex.y = tex.y * texSize / (float)atlasHeight;
    res.txi = tex.x;
    res.tyi = tex.y;
    return res;
}
