#include "../include/chunk.hpp"

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
    buff = new float[realBuffSize * 5];
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

bool Chunk::checkBlock(int index) const {
    auto it = data.find(index);
    return it != data.end() && it->second.getId() != 0;
}

bool Chunk::checkBlock(char x, char y, char z) const {
    // return data.find(getIndex(x, y, z)) != data.end();
    return checkBlock(getIndex(x, y, z));
}

bool Chunk::checkBlock(const glm::ivec3 &vec) const {
    return checkBlock((char)vec.x, (char)vec.y, (char)vec.z);
}

void Chunk::updateBuff() {
    if (data.size() == 0)
        return;

    // TODO: make better allocation strategy
    if (realBuffSize < data.size()) {
        delete[] buff;
        realBuffSize = data.size() + 16;
        buff = new float[realBuffSize * 5];
    }
    else if (realBuffSize - data.size() > 32) {
        delete[] buff;
        realBuffSize = data.size() + 8;
        buff = new float[realBuffSize * 5];
    }

    int ind = 0;
    for (const auto &e : data) {        
        char x, y, z;
        parseIndex(e.first, x, y, z);
        buff[ind++] = (float)x;
        buff[ind++] = (float)y;
        buff[ind++] = (float)z;
        buff[ind++] = (float)e.second.getTex().x;
        buff[ind++] = (float)e.second.getTex().y;
    }
}

const float* Chunk::getBuff() const {
    return buff;
}

size_t Chunk::getBuffSize() const {
    return data.size() * 5 * sizeof(float);
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