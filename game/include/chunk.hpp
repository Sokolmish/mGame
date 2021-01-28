#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "util/util.hpp"
#include "block.hpp"
#include <vector>
#include <map>

class Chunk {
private:
    glm::vec3 offset;
    std::map<short int, Block> data;

    static size_t getIndex(char x, char y, char z);
    static void parseIndex(size_t index, char &x, char &y, char &z);

    friend class GameWorld;
public:
    Chunk(int offx, int offz);
    ~Chunk();

    void setBlock(char x, char y, char z, const Block &block);
    Block getBlock(char x, char y, char z) const;
};

#endif
