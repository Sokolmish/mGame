#include "../include/item.hpp"

uint Item::atlasWidth = 128;
uint Item::atlasHeight = 128;
uint Item::texSize = 16;

Item::Item() {
    Id = 0;
}

Item::Item(uint16_t id, bool isBlock, int tx, int ty) {
    Id = id;
    _isBlock = isBlock;
    this->tx = tx * texSize / (float)atlasWidth;
    this->ty = ty * texSize / (float)atlasHeight;
    txi = tx;
    tyi = ty;
}

Block Item::toBlock() const {
    return Block(Id, { txi, tyi });
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
