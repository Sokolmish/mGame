#include "../include/gameWorld.hpp"
#include "../include/util/image.hpp"

GameWorld::GameWorld(int cwidth, int cheight) {
    cubeShader = Shader::getShader("cubeShader");

    this->cwidth = cwidth;
    this->cheight = cheight;

    int offx = -cwidth / 2;
    int offy = -cheight / 2;
    for (int i = 0; i < cwidth; i++)
        for (int j = 0; j < cheight; j++)
            chunks.insert({ { offx + i, offy + j }, Chunk((offx + i) * 16, (offy + j) * 16) });

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    size_t buffSize = 4096 * 5;
    buff = new GLfloat[buffSize];

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glBufferData(GL_ARRAY_BUFFER, buffSize * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    size_t stride = 5 * sizeof(GLfloat);
    glEnableVertexAttribArray(0); // Center's pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texture = Image::loadImage("blocks");
}

GameWorld::~GameWorld() {
    delete[] buff;
}

void GameWorld::showChunk(const Chunk &chunk, const glm::mat4 &m_proj_view) const {
    cubeShader.setUniform("chunkOffset", chunk.offset);

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
            ind = 0;
            count = 0;
        }
    }
    if (count != 0) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * 5 * sizeof(GLfloat), buff);
        glDrawArrays(GL_POINTS, 0, count);
    }
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

    for (const auto &chunk : chunks)
        showChunk(chunk.second, m_proj_view);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void GameWorld::setBlock(int x, int y, int z, const Block &block) {
    auto chunk = chunks.find({ eucl_div(x, 16), eucl_div(z, 16) });
    if (chunk != chunks.end())
        chunk->second.setBlock(eucl_mod(x, 16), y, eucl_mod(z, 16), block);
}

Block GameWorld::getBlock(int x, int y, int z) const {
    auto chunk = chunks.find({ eucl_div(x, 16), eucl_div(z, 16) });
    if (chunk != chunks.end())
        return chunk->second.getBlock(eucl_mod(x, 16), y, eucl_mod(z, 16));
    else
        return BLOCK_DAIR;
}

void GameWorld::setBlock(const glm::ivec3 &pos, const Block &block) {
    setBlock(pos.x, pos.y, pos.z, block);
}

Block GameWorld::getBlock(const glm::ivec3 &pos) const {
    return getBlock(pos.x, pos.y, pos.z);
}

bool GameWorld::checkBlock(int x, int y, int z) const {
    auto chunk = chunks.find({ eucl_div(x, 16), eucl_div(z, 16) });
    if (chunk == chunks.end())
        return false;
    auto it2 = chunk->second.data.find(Chunk::getIndex(eucl_mod(x, 16), y, eucl_mod(z, 16)));
    return it2 != chunk->second.data.end() && it2->second.getId() != 0;
}

bool GameWorld::checkBlock(const glm::ivec3 &vec) const {
    return checkBlock(vec.x, vec.y, vec.z);
}
