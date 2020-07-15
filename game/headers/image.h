#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>
#include <string>

class Image {
private:
    int width, height;
    uint8_t *data;
    bool isReleased;
public:
    Image(const std::string &path);
    ~Image();
    void release();
    uint8_t* getData();
    int getW();
    int getH();
};

#endif