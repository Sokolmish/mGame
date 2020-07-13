#include "glew.h"
#include <GLFW/glfw3.h>
#include "SOIL.h"

#include <iostream>
#include <string>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "param.h"
#include "shader.h"
#include "camera.h"
#include "chunk.h"
#include "font.h"

Camera cam;
double oldmx, oldmy;

// void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
// void window_size_callback(GLFWwindow*, int, int);

void pollMovement(GLFWwindow *window, float dt) {
    float coeff1 = 0.7f;
    float coeff2 = 1.8f;
    float coeff3 = -0.1f;

    glm::vec3 viewDir = glm::vec3(-sinf(cam.yaw), 0, cosf(cam.yaw));
    glm::vec3 leftDir = glm::vec3(cosf(cam.yaw), 0, sinf(cam.yaw));
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.pos -= coeff1 * dt * viewDir;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.pos += coeff1 * dt * viewDir;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.pos -= coeff1 * dt * leftDir;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.pos += coeff1 * dt * leftDir;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cam.pos.y += coeff1 * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cam.pos.y -= coeff1 * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cam.yaw = fmodf(cam.yaw - coeff2 * dt, 2 * M_PI);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cam.yaw = fmodf(cam.yaw + coeff2 * dt, 2 * M_PI);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cam.pitch = fmodf(cam.pitch + coeff2 * dt, 2 * M_PI);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) {
        cam.pitch = fmodf(cam.pitch - coeff2 * dt, 2 * M_PI);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        double dmx = mx - oldmx;
        double dmy = my - oldmy;
        oldmx = mx;
        oldmy = my;

        cam.yaw = fmodf(cam.yaw - (dmx * -coeff3 * dt), 2 * M_PI);
        cam.pitch = fmodf(cam.pitch - (dmy * coeff3 * dt), 2 * M_PI); 
    }
}

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
    unsigned char* image = SOIL_load_image("./game/textures/demoTexture.png", &imgWidth, &imgHeight, nullptr, SOIL_LOAD_RGB);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float ratio = (float)width / (float)height;
    glClearColor(0.509f, 0.788f, 0.902f, 1.f);
    // glfwSwapInterval(0);

    // glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetWindowSizeCallback(window, window_size_callback);

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

    Font font("./game/fonts/consola.ttf", 0, 48);
    //! END TEMP
    
    // Main loop
    float oTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float nTime = glfwGetTime();
        float dt = nTime - oTime;
        oTime = nTime;

        glfwPollEvents();
        pollMovement(window, dt);

        glfwGetWindowSize(window, &width, &height);
        ratio = (float)width / (float)height;
        glViewport(0, 0, width, height);
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Cube shader
        cubeShader.use();

        glm::mat4 m_proj_view =
            glm::perspective(45.f, ratio, 0.1f, 100.f) *
            glm::scale(glm::mat4(1.f), glm::vec3(cam.zoom, cam.zoom, 1.f)) *
            glm::rotate(glm::mat4(1.f), cam.roll, glm::vec3(0, 0, -1)) *
            glm::rotate(glm::mat4(1.f), cam.pitch, glm::vec3(-1, 0, 0)) *
            glm::rotate(glm::mat4(1.f), cam.yaw, glm::vec3(0, 1, 0)) *
            glm::translate(glm::mat4(1.f), -cam.pos);

        cubeShader.setUniform("m_proj_view", m_proj_view);
        cubeShader.setUniform("cubeHalfSize", 0.15f);

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
        
        font.RenderText(textShader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.f));
        font.RenderText(textShader, "This is sample text", 540.0f, 570.0f, 0.5f, glm::vec3(0.f));

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        glfwGetCursorPos(window, &oldmx, &oldmy);
}