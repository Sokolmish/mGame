#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "util/util.hpp"
#include "item.hpp"

class Item;

class Block {
private:
    uint16_t id;
    glm::vec2 tex;
    int txi, tyi;
public:
    static uint atlasWidth, atlasHeight;
    static uint texSize;

    Block();
    Block(short int id, const glm::vec2 &tex);

    Item toItem() const;

    uint16_t getId() const;
    glm::vec2 getTex() const;
};

#define BLOCK_DAIR      Block(0, { 0, 0 })
#define BLOCK_DGRASS    Block(1, { 0, 0 })
#define BLOCK_DSTONE    Block(2, { 1, 0 })
#define BLOCK_DWOOD     Block(3, { 2, 0 })

#endif