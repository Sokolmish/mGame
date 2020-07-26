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
#include "../include/gameWorld.hpp"

void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
// void cursor_position_callback(GLFWwindow*, double, double);
// void window_size_callback(GLFWwindow*, int, int);

bool mouseRightFlag = false;
bool mouseLeftFlag = false;

Player *tempPlayerRef;

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
    glBindVertexArray(0);
    delete[] buff;
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
    Shader wireShader = Shader::loadShader("wireShader");

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float ratio = (float)width / (float)height;
    glClearColor(0.509f, 0.788f, 0.902f, 1.f);
    glfwSwapInterval(0);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
    // glfwSetWindowSizeCallback(window, window_size_callback);

    Player player;
    GameWorld world;

    //! TEMP
    tempPlayerRef = &player;

    for (int xx = 0; xx < 16; xx++)
        for (int zz = 0; zz < 16; zz++)
            world.setBlock(xx, 2, zz, BLOCK_DGRASS);
    for (int xx = 7; xx < 10; xx++)
        for (int zz = 7; zz < 10; zz++)
            world.setBlock(xx, 5, zz, BLOCK_DSTONE);
    for (int yy = 0; yy < 16; yy++) 
        world.setBlock(2, yy, 2, BLOCK_DWOOD);
    for (int xx = 3; xx < 5; xx++) 
        for (int yy = 3; yy < 8; yy++) 
            world.setBlock(xx, yy, 2, BLOCK_DWOOD);
    for (int zz = 5; zz < 9; zz++)
        world.setBlock(5, 3, zz, BLOCK_DSTONE);
    for (int zz = 9; zz < 13; zz++)
        world.setBlock(5, 4, zz, BLOCK_DSTONE);
    world.setBlock(7, 6, 4, BLOCK_DSTONE);
    world.setBlock(7, 6, 3, BLOCK_DSTONE);
    world.setBlock(8, 6, 3, BLOCK_DSTONE);

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
        player.doPhysics(window, world, dt);

        glfwGetWindowSize(window, &width, &height);
        ratio = (float)width / (float)height;
        glViewport(0, 0, width, height);
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Cube shader
        Camera cam = player.getCamera();        
        glm::mat4 m_proj_view =
            glm::perspective(45.f, ratio, 0.005f, 100.f) *
            glm::scale(glm::mat4(1.f), glm::vec3(cam.zoom, cam.zoom, 1.f)) *
            glm::scale(glm::mat4(1.f), glm::vec3(0.3, 0.3, 0.3)) *
            glm::rotate(glm::mat4(1.f), cam.roll, glm::vec3(0, 0, -1)) *
            glm::rotate(glm::mat4(1.f), cam.pitch, glm::vec3(-1, 0, 0)) *
            glm::rotate(glm::mat4(1.f), cam.yaw, glm::vec3(0, 1, 0)) *
            glm::translate(glm::mat4(1.f), -cam.pos);
        
        world.show(m_proj_view);

        // Highlighting
        glm::ivec3 hiblock;
        WDir hiface;
        bool isBlockSelected = player.getSelectedBlock(world, hiblock, hiface);
        if (isBlockSelected) {
            wireShader.use();
            wireShader.setUniform("m_proj_view", m_proj_view);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);
            higlightBlock(highlightVao, highlightVbo, hiblock);
        }

        // Setting/destroying blocks
        if (mouseRightFlag) {
            mouseRightFlag = false;
            
            glm::ivec3 setPos = hiblock;
            if (hiface == NORTH)
                setPos.z--;
            else if (hiface == SOUTH)
                setPos.z++;
            else if (hiface == EAST)
                setPos.x++;
            else if (hiface == WEST)
                setPos.x--;
            else if (hiface == UP)
                setPos.y++;
            else
                setPos.y--;

            if (!world.checkBlock(setPos))
                world.setBlock(setPos, BLOCK_DSTONE);
        }
        else if (mouseLeftFlag) {
            mouseLeftFlag = false;
            if (world.checkBlock(hiblock))
                world.setBlock(hiblock, Block(0, { 0, 0 }));
        }

        // Debug layout
        debugLayout.setPos(player.getPos());
        debugLayout.setView(glm::degrees(player.getYaw()), glm::degrees(player.getPitch()));
        debugLayout.setGrounded(player.isGrounded(world));
        debugLayout.setFlightmoded(player.isFlight());
        debugLayout.setSelectedBlock(hiblock, hiface, isBlockSelected);
        debugLayout.show(width, height);

        // GUI layout
        guiLayout.show(width, height);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouseLeftFlag = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        mouseRightFlag = true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        tempPlayerRef->setFlight(!tempPlayerRef->isFlight());
    }
}

// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
// }