#include "item.hpp"

// Should be redefined in main!
uint Item::atlasWidth = 0;  // 128
uint Item::atlasHeight = 0; // 128
uint Item::texSize = 0;     // 16
std::map<uint16_t, Item> Item::itemsLib;

Item::Item() {
    Id = 0;
}

Item::Item(uint16_t id) {
    *this = itemsLib[id];
}

Block Item::toBlock() const {
    return Block(Id);
}

bool Item::isExist() const {
    return Id != 0;
}

bool Item::isItem() const {
    return isExist() && !_isBlock;
}

bool Item::isBlock() const {
    return isExist() && _isBlock;
}

Item Item::createItem(uint16_t id, bool isBlock, int tx, int ty) {
    Item res;
    res.Id = id;
    res._isBlock = isBlock;
    res.tx = tx * texSize / (float)atlasWidth;
    res.ty = ty * texSize / (float)atlasHeight;
    res.txi = tx;
    res.tyi = ty;
    return res;
}
