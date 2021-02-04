#include "../include/blocksHighlighter.hpp"

BlocksHighlighter::BlocksHighlighter() {
    shader = Shader::getShader("wireShader");

    GLfloat *buff = new GLfloat[48];
    for (size_t i = 24; i < 48; i++)
        buff[i] = 0.f;

    uint indices[16] = {
        2, 6, 7, 3, 2,
        1, 5, 6, 7,
        4, 5, 1, 0,
        3, 0, 4
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(GLfloat), buff, GL_DYNAMIC_DRAW); // TODO: STREAM?
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(24 * sizeof(GLfloat)));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] buff;
}

void BlocksHighlighter::show(const glm::mat4 &m_proj_view, const glm::vec3 &pos) const {
    float offset = 0.0002f;
    GLfloat vertices[8][3] = {
        {-0, -0, +1},
        {+1, -0, +1},
        {+1, +1, +1},
        {-0, +1, +1},
        {-0, -0, -0},
        {+1, -0, -0},
        {+1, +1, -0},
        {-0, +1, -0}
    };
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 3; j++)
            vertices[i][j] = pos[j] - offset + vertices[i][j] * (1 + offset + offset);

    shader.use();
    shader.setUniform("m_proj_view", m_proj_view);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glLineWidth(3);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(GLfloat), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(vao);
    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
