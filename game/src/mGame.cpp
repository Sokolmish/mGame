#include "../include/util/util.hpp"
#include "../include/util/shader.hpp"
#include "../include/mainMachine.hpp"
#include <iostream>
#include <string>

void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void window_size_callback(GLFWwindow*, int, int);

MainMachine *mainMachine;

int main() {
    GLFWwindow* window;
    if (!initGLFW(window) || !initGLEW())
        return -1;

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    Shader::shaderDirectory = "./game/shaders/";
    
    mainMachine = new MainMachine(window);
    mainMachine->enterMainLoop(); // Infinity loop

    delete mainMachine;
    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    mainMachine->clickMouse(button, action);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    mainMachine->clickKeyboard(key, action);
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    mainMachine->resize(width, height);
}
