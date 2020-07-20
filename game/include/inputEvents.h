#ifndef __INPUT_EVENTS_H__
#define __INPUT_EVENTS_H__

#include <GLFW/glfw3.h>
#include "player.h"

class InputPoller {
public:
    static double oldmx, oldmy;

    static float coeffMovement;
    static float coeffCameraKeyboard;
    static float coeffCameraMouse;

    static bool pollLooking(GLFWwindow *window, Player &player, float dt);
    static glm::vec3 pollMovement(GLFWwindow *window, Player &player, float dt);
};

#endif