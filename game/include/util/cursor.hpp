#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "util.hpp"

class Cursor {
private:
    int width, height;
    GLFWcursor *cursor;
public:
    Cursor(const std::string &path);
    void use(GLFWwindow *window) const;
};

#endif