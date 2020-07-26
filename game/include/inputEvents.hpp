#ifndef __INPUT_EVENTS_H__
#define __INPUT_EVENTS_H__

#include "../include/glew.hpp"
#include <GLFW/glfw3.h>
#include "player.hpp"

class InputPoller {
public:
    static double oldmx, oldmy;
    static double dmx, dmy;

    static float coeffMovement;
    static float coeffCameraKeyboard;
    static float coeffCameraMouse;

    static bool pollLooking(GLFWwindow *window, Player &player, float dt);
    static glm::vec3 pollMovement(GLFWwindow *window, Player &player, float dt);
};

#endif