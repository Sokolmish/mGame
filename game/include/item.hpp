#ifndef __ITEM_H__
#define __ITEM_H__

#include "util/util.hpp"
#include "block.hpp"
#include <map>

class Block;

struct Item {
    uint16_t Id;
    bool _isBlock; // or item
    float tx, ty; // texture coordinates
    int txi, tyi; // texture indices

    Item();
    explicit Item(uint16_t id);

    Block toBlock() const;

    bool isExist() const;
    bool isItem() const;
    bool isBlock() const;

    static uint atlasWidth, atlasHeight;
    static uint texSize;
    static std::map<uint16_t, Item> itemsLib;
    static Item createItem(uint16_t id, bool isBlock, int tx, int ty);
};

#endif
