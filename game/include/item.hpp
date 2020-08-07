#ifndef __ITEM_H__
#define __ITEM_H__

#include "util/util.hpp"
#include "block.hpp"

class Block;

struct Item {
    static uint atlasWidth, atlasHeight;
    static uint texSize;

    uint16_t Id;
    bool _isBlock; // or item
    float tx, ty; // texture coordinates
    int txi, tyi; // texture indices

    Item();
    Item(uint16_t id, bool isBlock, int tx, int ty);

    Block toBlock() const;

    bool isExist() const;
    bool isItem() const;
    bool isBlock() const;
};

#endif