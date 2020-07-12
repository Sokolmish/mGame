#include "chunk.h"

inline size_t getIndex(char x, char y, char z) {
    return (y * 16 + z) * 16 + x;
}

void parseIndex(size_t index, char &x, char &y, char &z) {
    x = index % 16;
    index /= 16;
    z = index % 16;
    index /= 16;
    y = index;
} 


Chunk::Chunk() {
    data = std::map<short int, Block>();
    realBuffSize = 4;
    buff = new uint32_t[realBuffSize * 4];
}

Chunk::~Chunk() {
    if (realBuffSize != 0)
        delete[] buff;
}

void Chunk::setBlock(char x, char y, char z, const Block &block) {
    data.insert_or_assign(getIndex(x, y, z), block);
    if (!noBuffUpdateFlag)
        updateBuff(); // TODO: smart update
}

Block Chunk::getBlock(char x, char y, char z) const {
    auto it = data.find(getIndex(x, y, z));
    if (it != data.end())
        return it->second;
    else
        return Block();
}

bool Chunk::checkBlock(char x, char y, char z) const {
    return data.find(getIndex(x, y, z)) != data.end();
}

bool Chunk::checkBlock(int index) const {
    return data.find(index) != data.end();
}

void Chunk::updateBuff() {
    if (data.size() == 0)
        return;

    // TODO: make better allocation strategy
    if (realBuffSize < data.size()) {
        delete[] buff;
        realBuffSize = data.size() + 16;
        buff = new uint32_t[realBuffSize * 4];
    }
    else if (realBuffSize - data.size() > 32) {
        delete[] buff;
        realBuffSize = data.size() + 8;
        buff = new uint32_t[realBuffSize * 4];
    }

    int ind = 0;
    for (const auto &e : data) {
        uint32_t val = e.second.getData();
        // TODO: event-based lighting sysytem
        if (checkBlock(e.first + 1)) // x+1
            val |= (1 << 21);
        if (checkBlock(e.first - 1)) // x-1
            val |= (1 << 22);
        if (checkBlock(e.first + 16)) // z+1
            val |= (1 << 23);
        if (checkBlock(e.first - 16)) // z-1
            val |= (1 << 24);
        if (checkBlock(e.first + 256)) // y+1
            val |= (1 << 25);
        if (checkBlock(e.first - 256)) // y-1
            val |= (1 << 26);
        
        char x, y, z;
        parseIndex(e.first, x, y, z);
        buff[ind++] = x;
        buff[ind++] = y;
        buff[ind++] = z;
        buff[ind++] = val;
    }
}

const uint32_t* Chunk::getBuff() const {
    return buff;
}

size_t Chunk::getBuffSize() const {
    return data.size() * 4 * sizeof(uint32_t);
}

size_t Chunk::getBlocksCount() const {
    return data.size();
}

void Chunk::startFilling() {
    noBuffUpdateFlag = true;
}

void Chunk::stopFilling() {
    noBuffUpdateFlag = false;
    updateBuff();
}