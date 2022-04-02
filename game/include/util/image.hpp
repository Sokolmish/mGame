#ifndef IMAGE_H_
#define IMAGE_H_

#include "util.hpp"
#include <string>
#include <map>

class Image {
private:
    int width, height;
    uint8_t *data;
    bool isReleased;

    static std::map<std::string, GLuint> staticStorage;
public:
    Image();
    explicit Image(const std::string &path);
    ~Image();

    void resize(int w, int h);

    uint8_t* getData() const;
    int getW() const;
    int getH() const;

    static void saveImage(const std::string &name, GLuint id);
    static GLuint loadImage(const std::string &name);
};

#endif
