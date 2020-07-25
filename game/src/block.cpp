#include "../include/block.hpp"

uint Block::atlasWidth = 128;
uint Block::atlasHeight = 128;
uint Block::texSize = 16;

Block::Block() {
    id = 0;
}

Block::Block(short int id, const glm::vec2 &tex) {
    this->id = id;
    this->tex = glm::vec2(tex.x * texSize / (float)atlasWidth, tex.y * texSize / (float)atlasHeight);
}


uint16_t Block::getId() const {
    return id;
}

glm::vec2 Block::getTex() const {
    return tex;
}