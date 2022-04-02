#ifndef __WIN_ICONS_H__
#define __WIN_ICONS_H__

#include "util.hpp"
#include "util/image.hpp"

class Cursor {
private:
//    int width, height;
    GLFWcursor *cursor;
public:
    explicit Cursor(const std::string &path);
    void use(GLFWwindow *window) const;
    // TODO: destructor?
    // TODO: prohibit copying?
};

class WindowIcon {
private:
    int size;
    GLFWimage **icons;
    Image **images;
public:
    // TODO: more than 1 icons
    explicit WindowIcon(const std::string &path);
    WindowIcon(WindowIcon const&) = delete;
    WindowIcon& operator=(WindowIcon const&) = delete;
    ~WindowIcon();
    void use(GLFWwindow *window) const;
};

#endif
