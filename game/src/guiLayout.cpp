#include "../include/guiLayout.hpp"

GuiLayout::GuiLayout(Interface *layout) {
    shader = Shader::getShader("guiShader");
    this->layout = layout;
    uint vertexCount = layout->getVertexCount();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * layout->getReserveBuffSize(), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 2 * vertexCount * sizeof(float), 4 * vertexCount * sizeof(float), layout->getColBuff());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * vertexCount * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GuiLayout::show(const glm::mat4 &m_ortho, float width, float height) const {
    shader.use();
    shader.setUniform("projection", m_ortho);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, layout->getVertexCount() * 2 * sizeof(float), layout->getLocBuff(width, height));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, layout->getVertexCount());
    glBindVertexArray(0);
}