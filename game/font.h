#ifndef __FONT_H__
#define __FONT_H__

#include "glew.h"

#include <string>
#include <vector>
#include <map>
#include <glm/vec2.hpp>
#include "shader.h"

class Font {
private:
    struct Character {
        GLuint    TextureID;
        // uint8_t     atlasX, atlasY;
        glm::ivec2  Size;
        glm::ivec2  Bearing;
        GLuint    Advance;
        // uint8_t *buff;
    };
    std::map<GLchar, Font::Character> Characters;
    GLuint VAO, VBO;
    // const uint8_t atlasWidth = 16;
    // const uint8_t atlasHeight = 8;
    // uint8_t *atlas;
    // std::vector<Character> chars;
public:
    Font(const std::string &path, uint32_t width, uint32_t height);
    ~Font();
    void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif