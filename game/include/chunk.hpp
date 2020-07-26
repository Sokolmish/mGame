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

    bool noBuffUpdateFlag = false; // mutable?

    static size_t getIndex(char x, char y, char z);
    static void parseIndex(size_t index, char &x, char &y, char &z);

    friend class GameWorld;
public:
    Chunk();
    ~Chunk();

    void setBlock(char x, char y, char z, const Block &block);
    Block getBlock(char x, char y, char z) const;
};

#endif