#include "../include/guiLayout.hpp"

GuiLayout::GuiLayout() {
    shader = Shader::getShader("guiShader");

    const int trigsCount = 4;
    GLfloat *buff = new GLfloat[6 * 3 * trigsCount];
    glm::vec4 col( 1.f, 1.f, 1.f, 1.f);
    for (size_t i = 0; i < 6 * trigsCount; i++)
        buff[i] = 0.5f;
    for (size_t i = 6 * trigsCount; i < 18 * trigsCount; i += 4) {
        buff[i + 0] = col.r;
        buff[i + 1] = col.g;
        buff[i + 2] = col.b;
        buff[i + 3] = col.a;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * trigsCount * sizeof(GLfloat), buff, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)((6 * trigsCount) * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    delete[] buff;
}

void GuiLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
    shader.use();
    shader.setUniform("projection", m_ortho);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat cx = width / 2.f;
    GLfloat cy = height / 2.f;
    GLfloat crw = 2.f;
    GLfloat cro = 10.f;
    GLfloat vertices[12][2] = {
        { cx - crw, cy + cro },
        { cx - crw, cy - cro },
        { cx + crw, cy - cro },
        { cx - crw, cy + cro },
        { cx + crw, cy - cro },
        { cx + crw, cy + cro },
        { cx - cro, cy + crw },
        { cx - cro, cy - crw },
        { cx + cro, cy - crw },
        { cx - cro, cy + crw },
        { cx + cro, cy - crw },
        { cx + cro, cy + crw },
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
}