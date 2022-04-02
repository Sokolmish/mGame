#ifndef INPUT_EVENTS_H_
#define INPUT_EVENTS_H_

#include "glew.hpp"
#include <GLFW/glfw3.h>
#include "player.hpp"

class InputPoller {
public:
    static double oldmx, oldmy;

    static float coeffMovement;
    static float coeffCameraKeyboard;
    static float coeffCameraMouse;

    static bool pollLooking(GLFWwindow *window, Player &player, float dt);
    static glm::vec3 pollMovement(GLFWwindow *window, const Player &player, float dt);
};

#endif
