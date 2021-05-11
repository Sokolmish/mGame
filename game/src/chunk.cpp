#include "chunk.hpp"

size_t Chunk::getIndex(char x, char y, char z) {
    return (y * 16 + z) * 16 + x;
}

void Chunk::parseIndex(size_t index, char &x, char &y, char &z) {
    x = index % 16;
    index /= 16;
    z = index % 16;
    index /= 16;
    y = index;
}


Chunk::Chunk(int offx, int offz) {
    data = std::map<uint16_t, Block>();
    offset = { offx, 0, offz };
}

Chunk::~Chunk() {

}

void Chunk::setBlock(char x, char y, char z, const Block &block) {
    data.insert_or_assign(getIndex(x, y, z), block);
}

Block Chunk::getBlock(char x, char y, char z) const {
    auto it = data.find(getIndex(x, y, z));
    if (it != data.end())
        return it->second;
    else
        return Block();
}

void Chunk::destroyBlock(char x, char y, char z) {
    data.erase(getIndex(x, y, z));
}
