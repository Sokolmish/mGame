#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "util/util.hpp"

class Block {
private:
    uint16_t id;
    glm::vec2 tex;
public:
    static uint atlasWidth, atlasHeight;
    static uint texSize;

    Block();
    Block(short int id, const glm::vec2 &tex);


    uint16_t getId() const;
    glm::vec2 getTex() const;
};

#define BLOCK_DGRASS    Block(1, { 0, 0 })
#define BLOCK_DSTONE    Block(1, { 1, 0 })
#define BLOCK_DWOOD     Block(1, { 2, 0 })

#endif