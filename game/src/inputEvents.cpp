#include "../include/inputEvents.hpp"
#include <cmath>
#include <glm/vec3.hpp>

double InputPoller::oldmx = 0.0;
double InputPoller::oldmy = 0.0;

float InputPoller::coeffMovement = 3.0f;
float InputPoller::coeffCameraKeyboard = 1.8f;
float InputPoller::coeffCameraMouse = 3.0f;

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

glm::vec3 InputPoller::pollMovement(GLFWwindow *window, Player &player, float dt) {
    // bool isUpdated = false;
    glm::vec3 moveDir = player.getMoveDir();                 // (-sinf(yaw), 0, cosf(yaw));
    glm::vec3 leftDir = glm::vec3(moveDir.z, 0, -moveDir.x); // (cosf(yaw), 0, sinf(yaw));  
    glm::vec3 res(0.f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        res += glm::vec3(-coeffMovement * dt * moveDir);
        // isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        res += glm::vec3(coeffMovement * dt * moveDir);
        // isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        res += glm::vec3(-coeffMovement * dt * leftDir);
        // isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        res += glm::vec3(coeffMovement * dt * leftDir);
        // isUpdated = true;
    }
    if (player.isFlight()) {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            res.y += coeffMovement * dt;
            // isUpdated = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            res.y += -coeffMovement * dt;
            // isUpdated = true;
        }
    }
    return res;
}

bool InputPoller::pollLooking(GLFWwindow *window, Player &player, float dt) {
    bool isUpdated = false;
    // Keyboard
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        player.setYaw(stepYaw(player.getYaw(), coeffCameraKeyboard * dt));
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        player.setYaw(stepYaw(player.getYaw(), -coeffCameraKeyboard * dt));
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        player.setPitch(stepPitch(player.getPitch(), -coeffCameraKeyboard * dt));
        isUpdated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) {
        player.setPitch(stepPitch(player.getPitch(), coeffCameraKeyboard * dt));
        isUpdated = true;
    }
    // Mouse
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    double dmx = mx - oldmx;
    double dmy = my - oldmy;
    oldmx = mx;
    oldmy = my;

    player.setYaw(stepYaw(player.getYaw(), dmx * -coeffCameraMouse * dt));
    player.setPitch(stepPitch(player.getPitch(), dmy * coeffCameraMouse * dt));

    isUpdated = true;
    return isUpdated;
}