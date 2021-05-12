#include "util/win_icons.hpp"

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
    size = 1;
    images = new Image*[size];
    icons = new GLFWimage*[size];

    images[0] = new Image(path);

    icons[0] = new GLFWimage();
    icons[0]->width = images[0]->getW();
    icons[0]->height = images[0]->getH();
    icons[0]->pixels = images[0]->getData();
}

WindowIcon::~WindowIcon() {
    for (int i = 0; i < size; i++) {
        delete icons[i];
        delete images[i];
    }
    delete[] icons;
    delete[] images;
}

void WindowIcon::use(GLFWwindow *window) const {
    glfwSetWindowIcon(window, size, *icons);
}
