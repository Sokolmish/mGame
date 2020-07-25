#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <vector>
#include <map>
#include <glm/vec3.hpp>

#include "block.hpp"

class Chunk {
private:
    std::map<short int, Block> data;
    float *buff;
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
    const float *getBuff() const;
    size_t getBuffSize() const;
    size_t getBlocksCount() const;

    void startFilling();
    void stopFilling();
};

#endif