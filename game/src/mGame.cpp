#include "../include/glew.hpp"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

#include "../include/param.hpp"
#include "../include/util.hpp"
#include "../include/inputEvents.hpp"
#include "../include/shader.hpp"
#include "../include/player.hpp"
#include "../include/chunk.hpp"
#include "../include/font.hpp"
#include "../include/image.hpp"
#include "../include/debugLayout.hpp"
#include "../include/guiLayout.hpp"

void key_callback(GLFWwindow*, int, int, int, int);
// void mouse_button_callback(GLFWwindow*, int, int, int);
// void cursor_position_callback(GLFWwindow*, double, double);
// void window_size_callback(GLFWwindow*, int, int);

Player *tempPlayerRef;

void loadChunk(GLuint vao, const Chunk &chunk) {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLfloat *buff = (GLfloat*)chunk.getBuff();
    glBufferData(GL_ARRAY_BUFFER, chunk.getBuffSize(), buff, GL_DYNAMIC_DRAW);
    size_t stride = 5 * sizeof(GLfloat);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(GLuint)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLuint)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void loadHighlighter(GLuint vao, GLuint vbo) {
    GLfloat *buff = new GLfloat[144];
    for (size_t i = 0; i < 72; i++) {
        buff[i + 0] = 0;
        buff[72 + i] = 0; // Black
    }
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * 6 * sizeof(GLfloat), buff, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(72 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    delete[] buff;
}

void higlightBlock(GLuint vao, GLuint vbo, const glm::ivec3 &pos) {
    glLineWidth(3);
    GLfloat *buff = new GLfloat[72];
    GLfloat vs[8][3] = {
        {-0, -0, +1},
        {+1, -0, +1},
        {+1, +1, +1},
        {-0, +1, +1},
        {-0, -0, -0},
        {+1, -0, -0},
        {+1, +1, -0},
        {-0, +1, -0}
    };
    int es[6][4] = {
        {2, 6, 7, 3},
        {4, 5, 1, 0},
        {7, 4, 0, 3},
        {2, 1, 5, 6},
        {6, 5, 4, 7},
        {3, 0, 1, 2}
    };
    int ind = 0;
    float offset = 0.0005f;
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 3; k++)
                buff[ind++] = pos[k] - offset + vs[es[i][j]][k] * (1 + offset + offset);
    // TODO: EBO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 72 * sizeof(GLfloat), buff);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    delete[] buff;
}

bool getSelectedBlock(const Chunk &chunk, const Player &player, glm::ivec3 &block, WDir &face) {
    const float maxlen = 3;
    glm::vec3 orig = player.getCamera().pos;
    glm::vec3 dir = player.getViewDir();
    RayIntersector ray(orig, dir);
    int dx = dir.x > 0 ? 1 : -1;
    int dy = dir.y > 0 ? 1 : -1;
    int dz = dir.z > 0 ? 1 : -1;
    for (int xx = 0; xx <= maxlen; xx++) {
        for (int zz = 0; zz <= maxlen; zz++) {
            for (int yy = 0; yy <= maxlen; yy++) {
                glm::ivec3 cur = glm::floor(orig + glm::vec3{ xx * dx, yy * dy, zz * dz });
                if (chunk.checkBlock(cur) && ray.intersect(cur, cur + glm::ivec3(1.f), face)) {
                    block = cur;
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    // Window init
    GLFWwindow* window;
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    std::string title = std::string(WINDOW_TITLE) + " | v" + std::string(VERSION);
    window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, title.c_str(), NULL, NULL);
    if (!window) {
        std::cout << "Failed to initialize window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLEW init
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK || !GLEW_VERSION_2_1) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Shaders loading
    Shader::shaderDirectory = "./game/shaders/";
    Shader cubeShader = Shader::loadShader("cubeShader");
    Shader wireShader = Shader::loadShader("wireShader");

    // Texture loading
    Image image("./game/textures/demoTexture.png");
    Block::atlasWidth = image.getW();
    Block::atlasHeight = image.getH();
    GLuint texture;
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

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float ratio = (float)width / (float)height;
    glClearColor(0.509f, 0.788f, 0.902f, 1.f);
    glfwSwapInterval(0);

    glfwSetKeyCallback(window, key_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
    // glfwSetWindowSizeCallback(window, window_size_callback);

    Player player;

    //! TEMP
    tempPlayerRef = &player;

    Chunk chunk;
    chunk.startFilling();
    for (int xx = 0; xx < 16; xx++)
        for (int zz = 0; zz < 16; zz++)
            chunk.setBlock(xx, 2, zz, BLOCK_DGRASS);
    for (int xx = 7; xx < 10; xx++)
        for (int zz = 7; zz < 10; zz++)
            chunk.setBlock(xx, 5, zz, BLOCK_DSTONE);
    for (int yy = 0; yy < 16; yy++) 
        chunk.setBlock(2, yy, 2, BLOCK_DWOOD);
    for (int xx = 3; xx < 5; xx++) 
        for (int yy = 3; yy < 8; yy++) 
            chunk.setBlock(xx, yy, 2, BLOCK_DWOOD);
    for (int zz = 5; zz < 9; zz++)
        chunk.setBlock(5, 3, zz, BLOCK_DSTONE);
    for (int zz = 9; zz < 13; zz++)
        chunk.setBlock(5, 4, zz, BLOCK_DSTONE);
    chunk.setBlock(7, 6, 4, BLOCK_DSTONE);
    chunk.setBlock(7, 6, 3, BLOCK_DSTONE);
    chunk.setBlock(8, 6, 3, BLOCK_DSTONE);
    chunk.stopFilling();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    loadChunk(vao, chunk); // TODO: check for empty buffer
    player.setPos(5.f, 7, 5.f);
    player.setYaw(M_PI_4);

    GLuint highlightVao, highlightVbo;
    glGenVertexArrays(1, &highlightVao);
    glGenBuffers(1, &highlightVbo);
    loadHighlighter(highlightVao, highlightVbo);

    //! END TEMP

    DebugLayout debugLayout;
    GuiLayout guiLayout;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Main loop
    float oTime = glfwGetTime(); // Used for physics, updates every frame
    float ooTime = oTime; // Used for fps counting, updates every second
    uint framesCounter = 0;
    while (!glfwWindowShouldClose(window)) {
        float nTime = glfwGetTime();
        float dt = nTime - oTime;
        oTime = nTime;

        framesCounter++;
        if (nTime - ooTime >= 1.0) {
            debugLayout.setFPS(framesCounter);
            framesCounter = 0;
            ooTime = nTime;
        }

        glfwPollEvents();
        bool isCameraUpdated = false;
        isCameraUpdated = InputPoller::pollLooking(window, player, dt) || isCameraUpdated;
        player.doPhysics(window, chunk, dt);

        glfwGetWindowSize(window, &width, &height);
        ratio = (float)width / (float)height;
        glViewport(0, 0, width, height);
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Cube shader
        cubeShader.use();

        Camera cam = player.getCamera();        
        glm::mat4 m_proj_view =
            glm::perspective(45.f, ratio, 0.005f, 100.f) *
            glm::scale(glm::mat4(1.f), glm::vec3(cam.zoom, cam.zoom, 1.f)) *
            glm::scale(glm::mat4(1.f), glm::vec3(0.3, 0.3, 0.3)) *
            glm::rotate(glm::mat4(1.f), cam.roll, glm::vec3(0, 0, -1)) *
            glm::rotate(glm::mat4(1.f), cam.pitch, glm::vec3(-1, 0, 0)) *
            glm::rotate(glm::mat4(1.f), cam.yaw, glm::vec3(0, 1, 0)) *
            glm::translate(glm::mat4(1.f), -cam.pos);

        cubeShader.setUniform("m_proj_view", m_proj_view);
        cubeShader.setUniform("cubeHalfSize", 1.f);
        cubeShader.setUniform("textureW", Block::texSize / static_cast<float>(Block::atlasWidth));
        cubeShader.setUniform("textureH", Block::texSize / static_cast<float>(Block::atlasHeight));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glDisable(GL_BLEND);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, chunk.getBlocksCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        // Highlighting
        glm::ivec3 hiblock;
        WDir hiface;
        bool isBlockSelected = getSelectedBlock(chunk, player, hiblock, hiface);
        if (isBlockSelected) {
            wireShader.use();
            wireShader.setUniform("m_proj_view", m_proj_view);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);
            higlightBlock(highlightVao, highlightVbo, hiblock);
        }

        // Debug layout

        debugLayout.setPos(player.getPos());
        debugLayout.setView(glm::degrees(player.getYaw()), glm::degrees(player.getPitch()));
        debugLayout.setGrounded(player.isGrounded(chunk));
        debugLayout.setFlightmoded(player.isFlight());
        debugLayout.setSelectedBlock(hiblock, hiface, isBlockSelected);
        debugLayout.show(width, height);

        // GUI layout

        guiLayout.show(width, height);

        //
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
// }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        tempPlayerRef->setFlight(!tempPlayerRef->isFlight());
    }
}

// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
// }