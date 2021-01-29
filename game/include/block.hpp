#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "util/util.hpp"
#include "item.hpp"
#include <map>

class Item;

class Block {
private:
    uint16_t id;
    glm::vec2 tex;
    int txi, tyi;
public:
    Block();
    Block(uint16_t id);

    Item toItem() const;

    uint16_t getId() const;
    glm::vec2 getTex() const;

    static uint atlasWidth, atlasHeight;
    static uint texSize;
    static std::map<uint16_t, Block> blocksLib;
    static Block createBlock(uint16_t id, const glm::vec2 &tex);
};

#endif
