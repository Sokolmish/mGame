#include "../headers/glew.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

#include "../headers/param.h"
#include "../headers/util.h"
#include "../headers/inputEvents.h"
#include "../headers/shader.h"
#include "../headers/camera.h"
#include "../headers/chunk.h"
#include "../headers/font.h"
#include "../headers/image.h"

// void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
// void window_size_callback(GLFWwindow*, int, int);

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
    int imgWidth, imgHeight;
    // unsigned char* image = SOIL_load_image("./game/textures/demoTexture.png", &imgWidth, &imgHeight, nullptr, SOIL_LOAD_RGB);
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
    // SOIL_free_image_data(image);
    image.release();
    glBindTexture(GL_TEXTURE_2D, 0);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float ratio = (float)width / (float)height;
    glClearColor(0.509f, 0.788f, 0.902f, 1.f);
    // glfwSwapInterval(0);

    // glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetWindowSizeCallback(window, window_size_callback);

    Camera cam;

    //! TEMP
    Chunk chunk;
    chunk.startFilling();
    for (int xx = 0; xx < 16; xx++) {
        for (int zz = 0; zz < 16; zz++) {
            chunk.setBlock(xx, 2, zz, Block(1));
        }
    }
    for (int yy = 0; yy < 32; yy++) 
        chunk.setBlock(1, yy, 1, Block(1));
    chunk.stopFilling();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    loadChunk(vao, chunk); // TODO: check for empty buffer
    cam.pos += glm::vec3(0.f, (30 * 0.15), 1.f);
    cam.yaw = M_PI / 4.f;

    Font font("./game/fonts/ConsolaMono-Bold.ttf", 0, 36);

    float nearVal = 0.01f;
    //! END TEMP
    
    // Main loop
    float oTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float nTime = glfwGetTime();
        float dt = nTime - oTime;
        oTime = nTime;

        glfwPollEvents();
        bool isCameraUpdated = false;
        isCameraUpdated = InputPoller::pollLooking(window, cam, dt) || isCameraUpdated;
        auto lastPos = cam.pos;
        isCameraUpdated = InputPoller::pollMovement(window, cam, dt) || isCameraUpdated;
        if (!(cam.pos.x > 15 || cam.pos.x < 0 || cam.pos.y > 15 || cam.pos.y < 0)) {
            // Assumes that last pos be in stable zone
            for (int i = 0; i < 3; i++) {
                if (fabsf(roundf(cam.pos[i]) - roundf(lastPos[i])) >= 1e-2) { // Zone II
                    glm::vec3 tdelta(0.f);
                    tdelta[i] = cam.pos[i] > lastPos[i] ? 1 : -1;
                    if (chunk.checkBlock(glm::round(glm::round(lastPos) + tdelta))) {
                        cam.pos[i] = roundf(lastPos[i]) + (-0.015 + 0.5) * (cam.pos[i] > lastPos[i] ? 1 : -1);
                    }
                }
            }
        }

        glfwGetWindowSize(window, &width, &height);
        ratio = (float)width / (float)height;
        glViewport(0, 0, width, height);
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Cube shader
        cubeShader.use();

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
        statusSS << "pos=" << cam.pos << ";";
        statusSS << "yaw=" << formatFloat("%.2f", glm::degrees(cam.yaw)) << ";";
        statusSS << "pitch=" << formatFloat("%.2f", glm::degrees(cam.pitch)) << ";";
        font.RenderText(textShader, statusSS.str(), 10, height - 22, 0.6, glm::vec3(0.f));
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        glfwGetCursorPos(window, &InputPoller::oldmx, &InputPoller::oldmy);
}