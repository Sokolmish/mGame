#ifndef __WIN_ICONS_H__
#define __WIN_ICONS_H__

#include "util.hpp"

class Cursor {
private:
    int width, height;
    GLFWcursor *cursor;
public:
    Cursor(const std::string &path);
    void use(GLFWwindow *window) const;
    // TODO: destructor?
};

class WindowIcon {
private:
    int size;
    GLFWimage *icons;
public:
    // TODO: more than 1 icons
    WindowIcon(const std::string &path);
    ~WindowIcon();
    void use(GLFWwindow *window) const;
};

#endif
