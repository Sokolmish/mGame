#ifndef __INPUT_EVENTS_H__
#define __INPUT_EVENTS_H__

#include <GLFW/glfw3.h>
#include "camera.h"

class InputPoller {
public:
    static double oldmx, oldmy;

    static float coeffMovement;
    static float coeffCameraKeyboard;
    static float coeffCameraMouse;

    static bool pollMovement(GLFWwindow *window, Camera &cam, float dt);
    static bool pollLooking(GLFWwindow *window, Camera &cam, float dt);
};

#endif