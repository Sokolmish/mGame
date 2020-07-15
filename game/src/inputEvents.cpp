#include "../headers/inputEvents.h"
#include <cmath>
#include <glm/vec3.hpp>

double InputPoller::oldmx = 0.0;
double InputPoller::oldmy = 0.0;

float InputPoller::coeffMovement = 3.5f;
float InputPoller::coeffCameraKeyboard = 1.8f;
float InputPoller::coeffCameraMouse = -0.1f;

inline float stepYaw(float yaw, float d) {
    yaw = fmodf(yaw - d, 2 * M_PI);
    if (yaw < 0)
        return 2 * M_PI + yaw;
    else
        return yaw;
}

inline float stepPitch(float pitch, float d) {
    pitch -= d;
    if (pitch > M_PI_2)
        return M_PI_2;
    else if (pitch < -M_PI_2)
        return -M_PI_2;
    else
        return pitch;
}

bool InputPoller::pollMovement(GLFWwindow *window, Camera &cam, float dt) {
    bool isUpdated = false;

    glm::vec3 viewDir = glm::vec3(-sinf(cam.yaw), 0, cosf(cam.yaw));
    glm::vec3 leftDir = glm::vec3(cosf(cam.yaw), 0, sinf(cam.yaw));
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.pos -= coeffMovement * dt * viewDir;
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.pos += coeffMovement * dt * viewDir;
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.pos -= coeffMovement * dt * leftDir;
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.pos += coeffMovement * dt * leftDir;
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cam.pos.y += coeffMovement * dt;
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cam.pos.y -= coeffMovement * dt;
        isUpdated = true;
    }
    
    return isUpdated;
}

bool InputPoller::pollLooking(GLFWwindow *window, Camera &cam, float dt) {
    bool isUpdated = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cam.yaw = stepYaw(cam.yaw, coeffCameraKeyboard * dt);
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cam.yaw = stepYaw(cam.yaw, -coeffCameraKeyboard * dt);
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cam.pitch = stepPitch(cam.pitch, -coeffCameraKeyboard * dt);
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) {
        cam.pitch = stepPitch(cam.pitch, coeffCameraKeyboard * dt);
        isUpdated = true;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        double dmx = mx - oldmx;
        double dmy = my - oldmy;
        oldmx = mx;
        oldmy = my;

        cam.yaw = stepYaw(cam.yaw, dmx * -coeffCameraMouse * dt);
        cam.pitch = stepPitch(cam.pitch, dmy * coeffCameraMouse * dt);

        isUpdated = true;
    }

    return isUpdated;
}