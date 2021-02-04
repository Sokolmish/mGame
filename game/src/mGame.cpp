#include "../include/util/util.hpp"
#include "../include/util/shader.hpp"
#include "../include/util/image.hpp"
#include "../include/util/win_icons.hpp"
#include "../include/mainMachine.hpp"
#include <iostream>
#include <string>

static void key_callback(GLFWwindow*, int, int, int, int);
static void mouse_button_callback(GLFWwindow*, int, int, int);
static void window_size_callback(GLFWwindow*, int, int);

static void loadTexture(GLuint Id, const Image &img) {
    glBindTexture(GL_TEXTURE_2D, Id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getW(), img.getH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

static MainMachine *mainMachine;

int main() {
    // Graphics initialization
    GLFWwindow *window;
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    std::string title = std::string(WINDOW_TITLE) + " | v" + std::string(VERSION);
    window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to initialize window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK || !GLEW_VERSION_2_1) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    // Shaders loading
    Shader::shaderDirectory = DEFAULT_SHADER_DIRECTORY;
    Shader::loadShader("cubeShader");
    Shader::loadShader("flatCShader");
    Shader::loadShader("flatTShader");
    Shader::loadShader("textShader");
    Shader::loadShader("wireShader");

    // Textures loading
    GLuint textures[3];
    Image *img;

    glGenTextures(sizeof(textures) / sizeof(GLuint), textures);
    Image::saveImage("blocks", textures[0]);
    Image::saveImage("items", textures[1]);
    Image::saveImage("interface", textures[2]);

    img = new Image("./game/textures/blocks.png");
    Block::texSize = BLOCK_TEXTURE_SIZE;
    Block::atlasWidth = img->getW();
    Block::atlasHeight = img->getH();
    loadTexture(textures[0], *img);
    delete img;

    img = new Image("./game/textures/items.png");
    Item::texSize = ITEM_TEXTURE_SIZE;
    Item::atlasWidth = img->getW();
    Item::atlasHeight = img->getH();
    loadTexture(textures[1], *img);
    delete img;

    img = new Image("./game/textures/interface.png");
    loadTexture(textures[2], *img);
    delete img;

    // Cursors loading
    Cursor expCursor("./game/textures/experimentalCursor1.png");
    expCursor.use(window);

    // Icon loading
    WindowIcon wIcon("./game/textures/experimentalIcon.png");
    wIcon.use(window);

    // Blocks creating
    Block::blocksLib = {
        { 1, Block::createBlock(1, { 0, 0 }) },
        { 2, Block::createBlock(2, { 1, 0 }) },
        { 3, Block::createBlock(3, { 2, 0 }) }
    };

    // Items creating
    Item::itemsLib = {
        { 1, Item::createItem(1, true, 0, 0) },
        { 2, Item::createItem(2, true, 1, 0) },
        { 3, Item::createItem(3, true, 2, 0) },
        { 101, Item::createItem(101, false, 0, 0) },
        { 102, Item::createItem(102, false, 2, 1) },
        { 103, Item::createItem(103, false, 4, 2) },
        { 104, Item::createItem(104, false, 5, 2) }
    };

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
