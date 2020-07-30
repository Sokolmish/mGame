#include "../include/mainMachine.hpp"
#include "../include/inputEvents.hpp"

float MainMachine::delayUse = 0.2;

MainMachine::MainMachine(GLFWwindow *window) {
    this->window = window;
    globalState = GlobalGameState::LOADING_SCREEN;
    
    hideCursor = true;
    if (hideCursor)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwGetWindowSize(window, &this->width, &this->height);
    this->ratio = (float)width / (float)height;

    float curTime = glfwGetTime();
    lastIntercationTime = curTime;
    lastAttackTime = curTime;

    //! TEMP
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
    //! TEMP
    player.setPos(5.f, 7, 5.f);
    player.setYaw(M_PI_4);

    globalState = GlobalGameState::SINGLE_GAME;
}

void MainMachine::enterMainLoop() {
    glfwSwapInterval(0);

    float timePhys = glfwGetTime();  // Used for physics, updates every frame
    float timeFPS = timePhys;        // Used for fps counting, updates every second
    uint framesCounter = 0;

    while (!glfwWindowShouldClose(window)) {
        // Time deltas
        float nTime = glfwGetTime();
        float dt = nTime - timePhys;
        timePhys = nTime;
        
        // FPS counting
        framesCounter++;
        if (nTime - timeFPS >= 1.0) {
            this->fps = framesCounter;
            framesCounter = 0;
            timeFPS = nTime;
        }

        glfwPollEvents();
        glViewport(0, 0, width, height);
    
        if (globalState == GlobalGameState::LOADING_SCREEN) {

        }
        else if (globalState == GlobalGameState::START_MENU) {

        }
        else if (globalState == GlobalGameState::SINGLE_PAUSE) {

        }
        else if (globalState == GlobalGameState::SINGLE_GAME) {
            glClearColor(0.509f, 0.788f, 0.902f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            InputPoller::pollLooking(window, player, dt);
            player.doPhysics(window, world, dt);

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

            if (canInteract()) {
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
                else if (hiface == DOWN)
                    setPos.y--;

                glm::vec3 pos = player.getPos();
                bool noInPlayer = (int)pos.y > setPos.y || setPos.y > (int)(pos.y + player.height) ||
                    (int)(pos.x - player.halfSize) > setPos.x || setPos.x > (int)(pos.x + player.halfSize) ||
                    (int)(pos.z - player.halfSize) > setPos.z || setPos.z > (int)(pos.z + player.halfSize);

                if (noInPlayer && !world.checkBlock(setPos)) {
                    world.setBlock(setPos, BLOCK_DSTONE);
                    lastIntercationTime = glfwGetTime();
                }
            }
            
            if (canAttack()) {
                if (world.checkBlock(hiblock)) {
                    world.setBlock(hiblock, Block(0, { 0, 0 }));
                    lastAttackTime = glfwGetTime();
                }
            }

            // Debug layout
            debugLayout.setPos(player.getPos());
            debugLayout.setView(glm::degrees(player.getYaw()), glm::degrees(player.getPitch()));
            debugLayout.setGrounded(player.isGrounded(world));
            debugLayout.setFlightmoded(player.isFlight());
            debugLayout.setSelectedBlock(hiblock, hiface, isBlockSelected);
            debugLayout.setFPS(this->fps);
            debugLayout.show(m_ortho, width, height);

            // GUI layout
            guiLayout.show(m_ortho, width, height);
        }

        glfwSwapBuffers(window);
    }

    //
}

void MainMachine::setState(GlobalGameState state) {
    this->globalState = state;
}

bool MainMachine::isKeyPressed(int code) const {
    return glfwGetKey(window, code) == GLFW_PRESS;
}

bool MainMachine::isMousePressed(MouseButton code) const {
    int t;
    if (code == MOUSE_LEFT)
        t = GLFW_MOUSE_BUTTON_LEFT;
    else if (code == MOUSE_RIGHT)
        t = GLFW_MOUSE_BUTTON_RIGHT;
    else if (code == MOUSE_MIDDLE)
        t = GLFW_MOUSE_BUTTON_MIDDLE;
    if (code != MOUSE_NONE)
        return glfwGetMouseButton(window, t) == GLFW_PRESS;
    else
        return false;
}

bool MainMachine::canInteract() {
    float curTime = glfwGetTime();
    if (isMousePressed(MOUSE_RIGHT) && curTime - lastIntercationTime > delayUse)
        return true;
    else
        return false;
}

bool MainMachine::canAttack() {
    float curTime = glfwGetTime();
    if (isMousePressed(MOUSE_LEFT) && curTime - lastAttackTime > delayUse)
        return true;
    else
        return false;
}

// Input

void MainMachine::clickMouse(int key, int action) {
    
}

void MainMachine::clickKeyboard(int key, int action) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        hideCursor = !hideCursor;
        if (hideCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        player.setFlight(!player.isFlight());
    }
}

void MainMachine::resize(int width, int height) {
    this->width = width;
    this->height = height;
    this->ratio = (float)width / (float)height;
}
