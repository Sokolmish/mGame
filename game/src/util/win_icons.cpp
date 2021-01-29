#include "../../include/util/win_icons.hpp"
#include "../../include/util/image.hpp"

// Cursor

Cursor::Cursor(const std::string &path) {
    Image loadedImage(path);
    GLFWimage image;
    image.width = loadedImage.getW();
    image.height = loadedImage.getH();
    image.pixels = loadedImage.getData();
    cursor = glfwCreateCursor(&image, 0, 0);
}

void Cursor::use(GLFWwindow *window) const {
    glfwSetCursor(window, cursor);
}

// WindowIcon

WindowIcon::WindowIcon(const std::string &path) {
    Image loadedImage(path);
    GLFWimage image;
    image.width = loadedImage.getW();
    image.height = loadedImage.getH();
    image.pixels = loadedImage.getData();
    
    size = 1;
    icons = new GLFWimage[size];
    icons[0] = image;
}

WindowIcon::~WindowIcon() {
    delete[] icons;
}

void WindowIcon::use(GLFWwindow *window) const {
    glfwSetWindowIcon(window, size, icons);
}
