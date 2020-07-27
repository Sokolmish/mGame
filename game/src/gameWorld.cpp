#include "../include/gameWorld.hpp"
#include "../include/image.hpp"
#include "../include/block.hpp"
#include "../include/camera.hpp"
#include "../include/util.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

GameWorld::GameWorld() : cubeShader(Shader::loadShader("cubeShader")) {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    size_t buffSize = 4096 * 5;
    buff = new GLfloat[buffSize];

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    
    glBufferData(GL_ARRAY_BUFFER, buffSize * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    size_t stride = 5 * sizeof(GLfloat);
    glEnableVertexAttribArray(0); // Center's pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(GLuint)));
    glEnableVertexAttribArray(1); // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLuint)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Image image("./game/textures/demoTexture.png");
    Block::atlasWidth = image.getW();
    Block::atlasHeight = image.getH();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getW(), image.getH(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    image.release();
    glBindTexture(GL_TEXTURE_2D, 0);
}

GameWorld::~GameWorld() {
    delete[] buff;
}

void GameWorld::show(const glm::mat4 &m_proj_view) const {
    cubeShader.use();

    cubeShader.setUniform("m_proj_view", m_proj_view);
    cubeShader.setUniform("cubeHalfSize", 1.f);
    cubeShader.setUniform("textureW", Block::texSize / static_cast<float>(Block::atlasWidth));
    cubeShader.setUniform("textureH", Block::texSize / static_cast<float>(Block::atlasHeight));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glDisable(GL_BLEND);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindTexture(GL_TEXTURE_2D, texture);

    uint count = 0;
    uint ind = 0;
    for (auto cube : chunk.data) {
        if (cube.second.getId() == 0)
            continue;

        char x, y, z;
        Chunk::parseIndex(cube.first, x, y, z);
        buff[ind++] = (float)x;
        buff[ind++] = (float)y;
        buff[ind++] = (float)z;
        buff[ind++] = (float)cube.second.getTex().x;
        buff[ind++] = (float)cube.second.getTex().y;
        
        count++;
        if (count == 4096) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, 4096 * 5 * sizeof(GLfloat), buff);
            glDrawArrays(GL_POINTS, 0, 4096);
            count = 0;
        }
    }
    if (count != 0) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * 5 * sizeof(GLfloat), buff);
        glDrawArrays(GL_POINTS, 0, count);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void GameWorld::setBlock(int x, int y, int z, const Block &block) {
    chunk.setBlock(x, y, z, block);
}

Block GameWorld::getBlock(int x, int y, int z) const {
    return chunk.getBlock(x, y, z);
}

void GameWorld::setBlock(const glm::ivec3 &pos, const Block &block) {
    setBlock(pos.x, pos.y, pos.z, block);
}

Block GameWorld::getBlock(const glm::ivec3 &pos) const {
    return getBlock(pos.x, pos.y, pos.z);
}



bool GameWorld::checkBlock(int index) const {
    auto it = chunk.data.find(index);
    return it != chunk.data.end() && it->second.getId() != 0;
}

bool GameWorld::checkBlock(char x, char y, char z) const {
    return checkBlock(Chunk::getIndex(x, y, z));
}

bool GameWorld::checkBlock(const glm::ivec3 &vec) const {
    return checkBlock((char)vec.x, (char)vec.y, (char)vec.z);
}