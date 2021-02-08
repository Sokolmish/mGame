#ifndef __MAIN_MACHINE_H__
#define __MAIN_MACHINE_H__

#include "util/util.hpp"
#include "util/shader.hpp"

#include "player.hpp"
#include "gameWorld.hpp"

#include "blocksHighlighter.hpp"
#include "gui/debugLayout.hpp"
#include "gui/interfaceLayout.hpp"
#include "gui/inventoryLayout.hpp"

#include <set>
#include <memory>

enum class GlobalGameState { LOADING_SCREEN, START_MENU, SINGLE_GAME, SINGLE_PAUSE };

class MainMachine {
private:
    GLFWwindow *window;
    int width, height;
    float ratio;

    GlobalGameState globalState;

    Player *player;
    GameWorld *world;

    std::unique_ptr<DebugLayout> debugLayout;
    std::unique_ptr<BlocksHighlighter> blocksSelectLayout;
    std::unique_ptr<InterfaceLayout> interfaceLayout;
    std::unique_ptr<InventoryLayout> inventoryLayout;

    uint fps;
    bool isCursorHided;
    bool isInterfaceOpened;

    bool isKeyPressed(int code) const;
    bool isMousePressed(MouseButton code) const;

    float lastIntercationTime;
    float lastAttackTime;
    bool canInteract() const;
    bool canAttack() const;

public:
    MainMachine(GLFWwindow *window);
    ~MainMachine();

    void enterMainLoop();

    void setState(GlobalGameState state);
    void setCursorHiding(bool isHide);

    void clickMouse(float x, float y, int key, int action);
    void clickKeyboard(int key, int action);
    void resize(int width, int height);

    void save(const std::string &path) const;
};

#endif
