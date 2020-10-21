#include "../../include/util/cursor.hpp"
#include "../../include/util/image.hpp"

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