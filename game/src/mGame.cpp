#include "../include/glew.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

#include "../include/param.h"
#include "../include/util.h"
#include "../include/inputEvents.h"
#include "../include/shader.h"
#include "../include/player.h"
#include "../include/chunk.h"
#include "../include/font.h"
#include "../include/image.h"

void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
// void window_size_callback(GLFWwindow*, int, int);

Player *tempPlayerRef;

void loadChunk(GLuint vao, const Chunk &chunk) {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint *buff = (GLuint*)chunk.getBuff();
    glBufferData(GL_ARRAY_BUFFER, chunk.getBuffSize(), buff, GL_DYNAMIC_DRAW);
    size_t stride = 4 * sizeof(uint32_t);
    glVertexAttribPointer(0, 3, GL_UNSIGNED_INT, GL_FALSE, stride, (void*)(0 * sizeof(GLuint)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, stride, (void*)(3 * sizeof(GLuint)));
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool checkGrounded(const Player &player, const Chunk &chunk) {
    glm::vec3 pos = player.getPos();
    for (int xx = pos.x - player.halfSize; xx <= pos.x + player.halfSize; xx++) {
        for (int zz = pos.z - player.halfSize; zz <= pos.z + player.halfSize; zz++) {
            if (fractf(pos.y) < 5e-3f && chunk.checkBlock(xx, pos.y - 1, zz))
                return true;
            else if (fractf(pos.y) > 1.f - 2e-3f && chunk.checkBlock(xx, pos.y, zz))
                return true;
        }
    }
    return false;
}

bool checkNewPos(const Player &player, const Chunk &chunk, const glm::vec3 &pos) {
    if (pos.x - player.halfSize < 0 || pos.x + player.halfSize > 16)
        return false;
    if (pos.z - player.halfSize < 0 || pos.z + player.halfSize > 16)
        return false;
    for (int yy = pos.y; yy <= pos.y + player.height; yy++) {
        for (int xx = pos.x - player.halfSize; xx <= pos.x + player.halfSize; xx++) {
            for (int zz = pos.z - player.halfSize; zz <= pos.z + player.halfSize; zz++) {
                if (chunk.checkBlock(xx, yy, zz)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void doPhysics(GLFWwindow *window, Player &player, Chunk &chunk, bool &isGrounded, float dt) {
    glm::vec3 delta = InputPoller::pollMovement(window, player, dt);
    glm::vec3 lastPos = player.getPos();

    isGrounded = checkGrounded(player, chunk);
    
    if (!player.isFlight()) {
        if (!isGrounded) {
            player.setAcceleration(glm::vec3(0, -9.81f, 0));
        }
        else {  
            player.setAcceleration(glm::vec3(0));
            player.setVelocity(glm::vec3(0));
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                player.setVelocity(glm::vec3(0, 4.8, 0));
        }
    }
    else {
        player.setAcceleration(glm::vec3(0));
    }

    if (player.getAcceleration() != glm::vec3(0)) {
        glm::vec3 dvel = player.getVelocity() + player.getAcceleration() * dt;
        player.setVelocity(dvel);
    }
    if (player.getVelocity() != glm::vec3(0.f)) {
        glm::vec3 dpos = player.getVelocity() * dt;
        delta += dpos;
    }

    int dimMoved = -1;
    while (glm::length(delta) >= 0.3) {
        glm::vec3 curDelta = 0.3f * glm::normalize(delta);
        delta -= curDelta;
        glm::vec3 pos3 = player.getPos();
        dimMoved = 0;
        for (int i = 0; i < 3; i++) {
            glm::vec3 td(0.f);
            td[i] = curDelta[i];
            if (checkNewPos(player, chunk, pos3 + td)) {
                pos3 += td;
                dimMoved++;
            }
        }
        player.setPos(pos3);
        if (dimMoved == 0)
            break;
    }

    if (dimMoved != 0) {
        glm::vec3 pos3 = lastPos;
        for (int i = 0; i < 3; i++) {
            glm::vec3 td(0.f);
            td[i] = delta[i];
            if (checkNewPos(player, chunk, pos3 + td))
                pos3 += td;
        }
        player.setPos(pos3);
    }
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
    Shader textShader = Shader::loadShader("textShader");

    // Texture loading
    Image image("./game/textures/demoTexture.png");
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    image.release();
    glBindTexture(GL_TEXTURE_2D, 0);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float ratio = (float)width / (float)height;
    glClearColor(0.509f, 0.788f, 0.902f, 1.f);
    glfwSwapInterval(0);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetWindowSizeCallback(window, window_size_callback);

    Player player;

    //! TEMP
    tempPlayerRef = &player;
    Chunk chunk;
    chunk.startFilling();
    for (int xx = 0; xx < 16; xx++)
        for (int zz = 0; zz < 16; zz++)
            chunk.setBlock(xx, 2, zz, Block(1));

    for (int xx = 7; xx < 10; xx++)
        for (int zz = 7; zz < 10; zz++)
            chunk.setBlock(xx, 5, zz, Block(1));


    for (int yy = 0; yy < 16; yy++) 
        chunk.setBlock(2, yy, 2, Block(1));
    for (int xx = 3; xx < 5; xx++) 
        for (int yy = 3; yy < 8; yy++) 
            chunk.setBlock(xx, yy, 2, Block(1));
    for (int zz = 5; zz < 9; zz++)
        chunk.setBlock(5, 3, zz, Block(1));
    for (int zz = 9; zz < 13; zz++)
        chunk.setBlock(5, 4, zz, Block(1));
    chunk.setBlock(7, 6, 4, Block(1));
    chunk.setBlock(7, 6, 3, Block(1));
    chunk.setBlock(8, 6, 3, Block(1));
    chunk.stopFilling();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    loadChunk(vao, chunk); // TODO: check for empty buffer
    player.setPos(5.f, 7, 5.f);
    player.setYaw(M_PI_4);

    Font font("./game/fonts/ConsolaMono-Bold.ttf", 0, 36);

    float nearVal = 0.005f;
    //! END TEMP
    
    // Main loop
    float oTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float nTime = glfwGetTime();
        float dt = nTime - oTime;
        oTime = nTime;

        glfwPollEvents();
        bool isCameraUpdated = false;
        isCameraUpdated = InputPoller::pollLooking(window, player, dt) || isCameraUpdated;
        bool isGrounded;
        doPhysics(window, player, chunk, isGrounded, dt);

        glfwGetWindowSize(window, &width, &height);
        ratio = (float)width / (float)height;
        glViewport(0, 0, width, height);
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Cube shader
        cubeShader.use();

        Camera cam = player.getCamera();        
        glm::mat4 m_proj_view =
            glm::perspective(45.f, ratio, nearVal, 100.f) *
            glm::scale(glm::mat4(1.f), glm::vec3(cam.zoom, cam.zoom, 1.f)) *
            glm::scale(glm::mat4(1.f), glm::vec3(0.3, 0.3, 0.3)) *
            glm::rotate(glm::mat4(1.f), cam.roll, glm::vec3(0, 0, -1)) *
            glm::rotate(glm::mat4(1.f), cam.pitch, glm::vec3(-1, 0, 0)) *
            glm::rotate(glm::mat4(1.f), cam.yaw, glm::vec3(0, 1, 0)) *
            glm::translate(glm::mat4(1.f), -cam.pos);

        cubeShader.setUniform("m_proj_view", m_proj_view);
        cubeShader.setUniform("cubeHalfSize", 1.f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_CCW);
        glDisable(GL_BLEND);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, chunk.getBlocksCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        // Text shader

        textShader.use();
        glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
        textShader.setUniform("projection", proj);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        std::stringstream statusSS;
        statusSS << "pos=" << player.getPos() << ";";
        statusSS << "yaw=" << formatFloat("%.2f", glm::degrees(player.getYaw())) << ";";
        statusSS << "pitch=" << formatFloat("%.2f", glm::degrees(player.getPitch())) << ";";
        font.RenderText(textShader, statusSS.str(), 10, height - 20, 0.5, glm::vec3(0.f));
        statusSS = std::stringstream();
        if (isGrounded)
            statusSS << "grounded;";
        if (player.isFlight())
            statusSS << "flightmod;";
        font.RenderText(textShader, statusSS.str(), 10, height - 38, 0.5, glm::vec3(0.f));
        
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        glfwGetCursorPos(window, &InputPoller::oldmx, &InputPoller::oldmy);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        tempPlayerRef->setFlight(!tempPlayerRef->isFlight());
    }
}