#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <vector>
#include <map>
#include <glm/vec3.hpp>

#include "block.h"

class Chunk {
private:
    std::map<short int, Block> data;
    uint32_t *buff;
    size_t realBuffSize;

    bool noBuffUpdateFlag = false;
public:
    Chunk();
    ~Chunk();

    void setBlock(char x, char y, char z, const Block &block);
    Block getBlock(char x, char y, char z) const;
    bool checkBlock(int index) const;
    bool checkBlock(char x, char y, char z) const;
    bool checkBlock(const glm::ivec3 &vec) const;

    void updateBuff();
    const uint32_t *getBuff() const;
    size_t getBuffSize() const;
    size_t getBlocksCount() const;

    void startFilling();
    void stopFilling();
};

#endif