#ifndef __BLOCKS_HIGHLIGHTER_H__
#define __BLOCKS_HIGHLIGHTER_H__

#include "glew.hpp"
#include "util/shader.hpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class BlocksHighlighter {
private:
    Shader shader;
    GLuint vao, vbo, ebo;
public:
    BlocksHighlighter();
    void show(const glm::mat4 &m_proj_view, const glm::vec3 &pos) const;
};

#endif
