#include "../include/util/util.hpp"
#include "../include/util/shader.hpp"
#include "../include/util/image.hpp"
#include "../include/mainMachine.hpp"
#include <iostream>
#include <string>

void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void window_size_callback(GLFWwindow*, int, int);

MainMachine *mainMachine;

void loadTexture(GLuint Id, const Image &img) {
    glBindTexture(GL_TEXTURE_2D, Id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getW(), img.getH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main() {
    // Graphics initialization
    GLFWwindow* window;
    if (!initGLFW(window) || !initGLEW())
        return -1;

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    // Shaders loading
    Shader::shaderDirectory = "./game/shaders/";
    Shader::loadShader("cubeShader");
    Shader::loadShader("flatCShader");
    Shader::loadShader("flatTShader");
    Shader::loadShader("textShader");
    Shader::loadShader("wireShader");

    // Textures loading
    GLuint textures[2];
    glGenTextures(sizeof(textures) / sizeof(GLuint), textures);
    Image::saveImage("blocks", textures[0]);
    Image::saveImage("items", textures[1]);
    
    Image img1("./game/textures/blocks.png");
    Block::atlasWidth = img1.getW();
    Block::atlasHeight = img1.getH();
    loadTexture(textures[0], img1);
    img1.release();

    Image img2("./game/textures/items.png");
    Item::atlasWidth = img2.getW();
    Item::atlasHeight = img2.getH();
    loadTexture(textures[1], img2);
    img2.release();
    
    // Main infinity loop
    mainMachine = new MainMachine(window);
    mainMachine->enterMainLoop();

    // Termination
    delete mainMachine;
    glfwTerminate();
    return 0;
}

// Input callbacks

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    mainMachine->clickMouse(button, action);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    mainMachine->clickKeyboard(key, action);
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    mainMachine->resize(width, height);
}
