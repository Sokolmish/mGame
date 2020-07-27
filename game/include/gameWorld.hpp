#ifndef __GAME_WORLD_H__
#define __GAME_WORLD_H__

#include "util.hpp"
#include "chunk.hpp"
#include "shader.hpp"

class GameWorld {
private:
    Shader cubeShader;
    GLuint cubeVAO, cubeVBO;
    GLuint texture;
    GLfloat *buff;

    Chunk chunk;
public:
    GameWorld();
    ~GameWorld();

    void show(const glm::mat4 &m_proj_view) const;

    void setBlock(int x, int y, int z, const Block &block);
    void setBlock(const glm::ivec3 &pos, const Block &block);
    Block getBlock(int x, int y, int z) const;
    Block getBlock(const glm::ivec3 &pos) const;

    bool checkBlock(int index) const;
    bool checkBlock(char x, char y, char z) const;
    bool checkBlock(const glm::ivec3 &vec) const;
};

#endif