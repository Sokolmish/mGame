#include "mainMachine.hpp"
#include "inputEvents.hpp"
#include "gameSaver.hpp"

MainMachine::MainMachine(GLFWwindow *window) {
    this->window = window;
    setState(GlobalGameState::LOADING_SCREEN);

    player = new Player();
    world = new GameWorld(3, 3);

    debugLayout = std::make_unique<DebugLayout>();
    blocksSelectLayout = std::make_unique<BlocksHighlighter>();
    interfaceLayout = std::make_unique<InterfaceLayout>(player);
    inventoryLayout = std::make_unique<InventoryLayout>(player);

    glfwGetWindowSize(window, &this->width, &this->height);
    this->ratio = (float)width / (float)height;

    GameSaver loader("./saves/dev1");
    loader.loadChunk(0, 0, world->getChunks().at(std::make_pair(0, 0)));
    loader.loadPlayer(*player);

    float curTime = static_cast<float>(glfwGetTime());
    lastIntercationTime = curTime;
    lastAttackTime = curTime;

    setState(GlobalGameState::SINGLE_GAME);
    isInterfaceOpened = false;
    setCursorHiding(false);
}

void MainMachine::enterMainLoop() {
    // glfwSwapInterval(0);

    float timePhys = static_cast<float>(glfwGetTime()); // Used for physics, updates every frame
    float timeFPS = timePhys; // Used for fps counting, updates every second
    uint framesCounter = 0;
    while (!glfwWindowShouldClose(window)) {
        // Time deltas
        float nTime = static_cast<float>(glfwGetTime());
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

        // TODO: switched table or something else
        if (globalState == GlobalGameState::LOADING_SCREEN) {
            // TODO
        }
        // else if (globalState == GlobalGameState::START_MENU) {
        //     // TODO
        // }
        // else if (globalState == GlobalGameState::SINGLE_PAUSE) {
        //     // TODO
        // }
        else if (globalState == GlobalGameState::SINGLE_GAME) {
            glClearColor(0.509f, 0.788f, 0.902f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::vec3 delta(0.f);
            if (!isInterfaceOpened) {
                InputPoller::pollLooking(window, *player, dt);
                delta = InputPoller::pollMovement(window, *player, dt);
            }
            if (!isInterfaceOpened && !player->isFlight() && player->isGrounded(*world)) {
                player->setAcceleration(glm::vec3(0));
                player->setVelocity(glm::vec3(0));
                // TODO: make jump method
                if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                    player->setVelocity(glm::vec3(0, 4.85, 0));
            }
            player->doPhysics(*world, dt, delta);

            // Matrices
            Camera cam = player->getCamera();
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
            world->show(m_proj_view);

            // Interaction with world
            glm::ivec3 hiblock;
            WDir hiface;
            bool isBlockSelected = player->getSelectedBlock(*world, hiblock, hiface);
            if (!isInterfaceOpened) {
                if (isBlockSelected) {
                    blocksSelectLayout->show(m_proj_view, hiblock);

                    if (canInteract()) {
                        if (player->getSelectedItem().isBlock()) {
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

                            const glm::vec3 pos = player->getPos();
                            const float plHalfSize = player->halfSize;
                            const bool noInPlayer =
                                nfloorf(pos.y) > setPos.y || setPos.y > nfloorf(pos.y + player->height) ||
                                nfloorf(pos.x - plHalfSize) > setPos.x || setPos.x > nfloorf(pos.x + plHalfSize) ||
                                nfloorf(pos.z - plHalfSize) > setPos.z || setPos.z > nfloorf(pos.z + plHalfSize);

                            if (noInPlayer && !world->checkBlock(setPos)) {
                                world->setBlock(setPos, player->getSelectedItem().toBlock());
                                lastIntercationTime = static_cast<float>(glfwGetTime());
                            }
                        }
                    }

                    if (canAttack()) {
                        if (world->checkBlock(hiblock)) {
                            world->destroyBlock(hiblock);
                            lastAttackTime = static_cast<float>(glfwGetTime());
                        }
                    }
                }
            }

            float fwidth = static_cast<float>(width);
            float fheight = static_cast<float>(height);

            // Debug layout
            debugLayout->setPos(player->getPos());
            debugLayout->setView(glm::degrees(player->getYaw()), glm::degrees(player->getPitch()));
            debugLayout->setGrounded(player->isGrounded(*world));
            debugLayout->setFlightmoded(player->isFlight());
            debugLayout->setSelectedBlock(hiblock, hiface, isBlockSelected);
            debugLayout->setFPS(this->fps);
            debugLayout->show(m_ortho, fwidth, fheight);

            // Interface
            if (!isInterfaceOpened) {
                interfaceLayout->changeGeometry(fwidth, fheight);
                interfaceLayout->updateSidebarItems(player->sidebar);
                interfaceLayout->selectSidebarCell(player->getSelectedCell());
                interfaceLayout->show(m_ortho);
            }
            else {
                inventoryLayout->changeGeometry(fwidth, fheight);
                inventoryLayout->show(m_ortho);
            }
        }

        glfwSwapBuffers(window);
    }
}

void MainMachine::setState(GlobalGameState state) {
    this->globalState = state;
}

bool MainMachine::isKeyPressed(int code) const {
    return glfwGetKey(window, code) == GLFW_PRESS;
}

bool MainMachine::isMousePressed(MouseButton code) const {
    int t = -1;
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

bool MainMachine::canInteract() const {
    float curTime = static_cast<float>(glfwGetTime());
    if (isMousePressed(MOUSE_RIGHT) && curTime - lastIntercationTime > DELAY_AFTER_USE)
        return true;
    else
        return false;
}

bool MainMachine::canAttack() const {
    float curTime = static_cast<float>(glfwGetTime());
    if (isMousePressed(MOUSE_LEFT) && curTime - lastAttackTime > DELAY_AFTER_DESTROY)
        return true;
    else
        return false;
}

void MainMachine::setCursorHiding(bool isHide) {
    isCursorHided = isHide;
    if (isCursorHided)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    InputPoller::oldmx = mx;
    InputPoller::oldmy = my;
}

// Saving

void MainMachine::save(const std::string &path) const {
    GameSaver saver(path);
    for (const auto &[pos, chunk] : world->getChunksData()) {
        if (!saver.saveChunk(pos.first, pos.second, chunk)) {
            std::cout << "Chunk saving error: ("
                    << pos.first << "," << pos.second
                    << ")" << std::endl;
        }
    }
    saver.savePlayer(*player);
}

// Input

void MainMachine::clickMouse(float x, float y, int key, int action) {
    if (isInterfaceOpened) {
        inventoryLayout->click(x, y, key, action);
    }
}

void MainMachine::clickKeyboard(int key, int action) {
    for (int i = 0; i < 9; i++) {
        if (key == GLFW_KEY_1 + i && action == GLFW_PRESS) {
            // int numPressed = i + 1;
            player->selectItem(i);
            return;
        }
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setCursorHiding(!isCursorHided);
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        player->setFlight(!player->isFlight());
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        isInterfaceOpened = !isInterfaceOpened;
        setCursorHiding(!isInterfaceOpened);
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        save("./saves/dev1");
    }
}

void MainMachine::resize(int width_, int height_) {
    this->width = width_;
    this->height = height_;
    this->ratio = (float)width / (float)height;
}

MainMachine::~MainMachine() {
    delete player;
    delete world;
}
