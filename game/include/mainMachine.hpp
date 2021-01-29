#ifndef __MAIN_MACHINE_H__
#define __MAIN_MACHINE_H__

#include "util/util.hpp"
#include "util/shader.hpp"

#include "player.hpp"
#include "gameWorld.hpp"

#include "debugLayout.hpp"
#include "blocksHighlighter.hpp"
#include "interfaceLayout.hpp"
#include "inventoryLayout.hpp"

#include <set>

enum class GlobalGameState { LOADING_SCREEN, START_MENU, SINGLE_GAME, SINGLE_PAUSE };

class MainMachine {
private:
    GLFWwindow *window;
    int width, height;
    float ratio;

    GlobalGameState globalState;

    Player *player;
    GameWorld *world;

    DebugLayout debugLayout;
    BlocksHighlighter blocksSelectLayout;
    InterfaceLayout interfaceLayout;
    InventoryLayout inventoryLayout;

    uint fps;
    bool hideCursor;

    bool isKeyPressed(int code) const;
    bool isMousePressed(MouseButton code) const;

    float lastIntercationTime;
    float lastAttackTime;
    bool canInteract() const;
    bool canAttack() const;

    bool interfaceOpened;

public:
    MainMachine(GLFWwindow *window);
    ~MainMachine();

    void enterMainLoop();

    void setState(GlobalGameState state);
    void setCursorHiding(bool isHide);

    void clickMouse(int key, int action);
    void clickKeyboard(int key, int action);
    void resize(int width, int height);

    void save(const std::string &path) const;
};

#endif
