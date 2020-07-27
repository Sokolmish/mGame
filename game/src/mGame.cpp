#include "../include/glew.hpp"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

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
#include "../include/blocksHighlighter.hpp"

void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
// void cursor_position_callback(GLFWwindow*, double, double);
// void window_size_callback(GLFWwindow*, int, int);

bool mouseRightFlag = false;
bool mouseLeftFlag = false;

Player *tempPlayerRef;

void tempInit(GameWorld &world, Player &player) {
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
}

int main() {
    GLFWwindow* window;
    if (!initGLFW(window) || !initGLEW())
        return -1;

    Shader::shaderDirectory = "./game/shaders/";

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
    // glfwSetWindowSizeCallback(window, window_size_callback);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float ratio = (float)width / (float)height;
    glClearColor(0.509f, 0.788f, 0.902f, 1.f);
    glfwSwapInterval(0);

    Player player;
    GameWorld world;

    tempInit(world, player);

    DebugLayout debugLayout;
    GuiLayout guiLayout;
    BlocksHighlighter blocksSelectLayout;    

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

        // Matrices
        Camera cam = player.getCamera();        
        glm::mat4 m_proj_view =
            glm::perspective(45.f, ratio, 0.005f, 100.f) *
            glm::scale(glm::mat4(1.f), glm::vec3(0.3, 0.3, 0.3)) *
            glm::scale(glm::mat4(1.f), glm::vec3(cam.zoom, cam.zoom, 1.f)) *
            glm::rotate(glm::mat4(1.f), cam.roll, glm::vec3(0, 0, -1)) *
            glm::rotate(glm::mat4(1.f), cam.pitch, glm::vec3(-1, 0, 0)) *
            glm::rotate(glm::mat4(1.f), cam.yaw, glm::vec3(0, 1, 0)) *
            glm::translate(glm::mat4(1.f), -cam.pos);
        glm::mat4 m_ortho = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
        
        // Cube shader
        world.show(m_proj_view);

        // Highlighting
        glm::ivec3 hiblock;
        WDir hiface;
        bool isBlockSelected = player.getSelectedBlock(world, hiblock, hiface);
        if (isBlockSelected)
            blocksSelectLayout.show(m_proj_view, hiblock);

        // Setting/destroying blocks
        if (mouseLeftFlag && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
            mouseLeftFlag = false;
        if (mouseRightFlag && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
            mouseRightFlag = false;

        if (mouseRightFlag) {
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

            glm::vec3 pos = player.getPos();
            bool noInPlayer = (int)pos.y > setPos.y || setPos.y > (int)(pos.y + player.height) ||
                (int)(pos.x - player.halfSize) > setPos.x || setPos.x > (int)(pos.x + player.halfSize) ||
                (int)(pos.z - player.halfSize) > setPos.z || setPos.z > (int)(pos.z + player.halfSize);

            if (noInPlayer && !world.checkBlock(setPos)) {
                world.setBlock(setPos, BLOCK_DSTONE);
                mouseRightFlag = false;
            }
        }
        else if (mouseLeftFlag) {
            if (world.checkBlock(hiblock)) {
                world.setBlock(hiblock, Block(0, { 0, 0 }));
                mouseLeftFlag = false;
            }
        }

        // Debug layout
        debugLayout.setPos(player.getPos());
        debugLayout.setView(glm::degrees(player.getYaw()), glm::degrees(player.getPitch()));
        debugLayout.setGrounded(player.isGrounded(world));
        debugLayout.setFlightmoded(player.isFlight());
        debugLayout.setSelectedBlock(hiblock, hiface, isBlockSelected);
        debugLayout.show(m_ortho, width, height);

        // GUI layout
        guiLayout.show(m_ortho, width, height);

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