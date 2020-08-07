#ifndef __MAIN_MACHINE_H__
#define __MAIN_MACHINE_H__

#include "util/util.hpp"
#include "util/shader.hpp"

#include "player.hpp"
#include "gameWorld.hpp"

#include "debugLayout.hpp"
#include "blocksHighlighter.hpp"
#include "interfaceLayout.hpp"

#include <set>

enum class GlobalGameState { LOADING_SCREEN, START_MENU, SINGLE_GAME, SINGLE_PAUSE };

class MainMachine {
private:
    static float delayUse;

    GLFWwindow *window;
    int width, height;
    float ratio;

    GlobalGameState globalState;
    
    // std::set<int> pressedKeys;
    // bool isRightClick;
    // bool isLeftClick;

    Player *player;
    GameWorld *world;

    DebugLayout debugLayout;
    BlocksHighlighter blocksSelectLayout;
    InterfaceLayout interfaceLayout;

    uint fps;
    bool hideCursor;

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

    void clickMouse(int key, int action);
    void clickKeyboard(int key, int action);
    void resize(int width, int height);
};

#endif