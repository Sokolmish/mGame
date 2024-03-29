#include "util/shader.hpp"

#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "util/util.hpp"

#define SHADER_INFOLOG_BUFSIZE 1024

std::string Shader::shaderDirectory = "./";
std::map<std::string, Shader> Shader::shaderStorage;

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath) {
    std::string src;
    const GLchar *ptr;
    GLint vSucc, fSucc, gSucc;
    GLuint vertexId, fragmentId, geometryId = 0;
    bool hasGeometry = geometryPath.length() != 0;
    // Vertex shader
    src = readTextFile(vertexPath);
    ptr = src.c_str();
    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &ptr, nullptr);
    glCompileShader(vertexId);
    // Fragment shader
    src = readTextFile(fragmentPath);
    ptr = src.c_str();
    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &ptr, nullptr);
    glCompileShader(fragmentId);
    // Geometry shader
    if (hasGeometry) {
        src = readTextFile(geometryPath);
        ptr = src.c_str();
        geometryId = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryId, 1, &ptr, nullptr);
        glCompileShader(geometryId);
    }
    // Shaders status
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &vSucc);
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &fSucc);
    if (hasGeometry)
        glGetShaderiv(geometryId, GL_COMPILE_STATUS, &gSucc);
    else
        gSucc = true;
    if (!(vSucc && fSucc && gSucc)) {
        // TODO: make the shaderException class
        GLchar infoLog[SHADER_INFOLOG_BUFSIZE];
        GLsizei logSize;
        if (!vSucc) {
            glGetShaderInfoLog(vertexId, SHADER_INFOLOG_BUFSIZE, &logSize, infoLog);
            std::cout << "Vertex shader compile error:" << std::endl << infoLog << std::endl;
            if (logSize >= SHADER_INFOLOG_BUFSIZE)
                std::cout << "======== infolog cutted ========" << std::endl;
        }
        if (!fSucc) {
            glGetShaderInfoLog(fragmentId, SHADER_INFOLOG_BUFSIZE, &logSize, infoLog);
            std::cout << "Fragment shader compile error:" << std::endl << infoLog << std::endl;
            if (logSize >= SHADER_INFOLOG_BUFSIZE)
                std::cout << "======== infolog cutted ========" << std::endl;
        }
        if (!gSucc) {
            glGetShaderInfoLog(geometryId, SHADER_INFOLOG_BUFSIZE, &logSize, infoLog);
            std::cout << "Geometry shader compile error:" << std::endl << infoLog << std::endl;
            if (logSize >= SHADER_INFOLOG_BUFSIZE)
                std::cout << "======== infolog cutted ========" << std::endl;
        }
        throw std::runtime_error("Shader compile error");
    }
    // Shader linking
    programId = glCreateProgram();
    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragmentId);
    if (hasGeometry)
        glAttachShader(programId, geometryId);
    glLinkProgram(programId);
    // Final shader status
    GLint progSucc;
    glGetProgramiv(programId, GL_LINK_STATUS, &progSucc);
    if (!progSucc) {
        // TODO: make the shaderException class
        GLchar infoLog[SHADER_INFOLOG_BUFSIZE];
        GLsizei logSize;
        glGetProgramInfoLog(programId, SHADER_INFOLOG_BUFSIZE, &logSize, infoLog);
        std::cout << "Shader linking error:" << std::endl << infoLog << std::endl;
        if (logSize >= SHADER_INFOLOG_BUFSIZE)
            std::cout << "======== infolog cutted ========" << std::endl;
        throw std::runtime_error("Shader linking error");
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    if (hasGeometry)
        glDeleteShader(geometryId);

    initialized = true;
}

Shader::Shader() {
    initialized = false;
}

inline bool checkFileExist(const std::string &name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else
        return false;
}

Shader Shader::loadShader(const std::string &name) {
    std::string vName = shaderDirectory + name + "/vert.glsl";
    std::string fName = shaderDirectory + name + "/frag.glsl";
    std::string gName = shaderDirectory + name + "/geom.glsl";
    if (!checkFileExist(vName))
        throw std::runtime_error("Missing file: " + vName);
    if (!checkFileExist(fName))
        throw std::runtime_error("Missing file: " + vName);
    bool hasGeometryShader = checkFileExist(gName);
    Shader s;
    if (hasGeometryShader)
        s = Shader(vName, fName, gName);
    else
        s = Shader(vName, fName);
    saveShader(name, s);
    return s;
}

void Shader::saveShader(const std::string &name, const Shader &shader) {
    shaderStorage.insert_or_assign(name, shader);
}

Shader Shader::getShader(const std::string &name) {
    auto it = shaderStorage.find(name);
    if (it == shaderStorage.end())
        return Shader();
    else
        return it->second;
}

//

void Shader::use() const {
    glUseProgram(programId);
}

GLuint Shader::getProgramId() const {
    return programId;
}

// Uniforms

void Shader::setUniform(const std::string &name, GLfloat val) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniform1f(uniformLoc, val);
}

void Shader::setUniform(const std::string &name, GLfloat v1, GLfloat v2) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniform2f(uniformLoc, v1, v2);
}
void Shader::setUniform(const std::string &name, GLfloat v1, GLfloat v2, GLfloat v3) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniform3f(uniformLoc, v1, v2, v3);
}
void Shader::setUniform(const std::string &name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniform4f(uniformLoc, v1, v2, v3, v4);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &val) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniform2f(uniformLoc, val.x, val.y);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &val) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniform3f(uniformLoc, val.x, val.y, val.z);
}

void Shader::setUniform(const std::string &name, const glm::vec4 &val) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniform4f(uniformLoc, val[0], val[1], val[2], val[3]);
}

void Shader::setUniform(const std::string &name, const glm::mat4 &val) const {
    GLuint uniformLoc = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(val));
}
