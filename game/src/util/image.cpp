#include "../../include/util/image.hpp"

#define STBI_NO_PSD
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/util/stb_image.hpp"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../../include/util/stb_image_resize.hpp"

#include <cstring>
#include <cassert>

std::map<std::string, GLuint> Image::staticStorage;

Image::Image() {
    width = 0;
    height = 0;
    data = nullptr;
    isReleased = true;
}

Image::Image(const std::string &path) {
    int n;
    uint8_t *buff = stbi_load(path.c_str(), &width, &height, &n, 4);
    assert(n == 4);
    int buffSize = width * height * 4;
    data = new uint8_t[buffSize];
    std::memcpy(data, buff, buffSize);
    stbi_image_free(buff);
    isReleased = false;
}

Image::~Image() {
    if (!isReleased) {
        delete[] data;
        isReleased = true;
    }
}


void Image::resize(int w, int h) {
    if (width != w || height != h) {
        uint8_t *temp = new uint8_t[w * h * 4];
        stbir_resize_uint8(data, width, height, 0, temp, w, h, 0, 4);
        delete[] data;
        data = temp;
        width = w;
        height = h;
    }
}


uint8_t* Image::getData() const {
    return data;
}

int Image::getW() const {
    return width;
}

int Image::getH() const {
    return height;
}


void Image::saveImage(const std::string &name, GLuint id) {
    staticStorage.insert_or_assign(name, id);
}

GLuint Image::loadImage(const std::string &name) {
    auto it = staticStorage.find(name);
    if (it == staticStorage.end())
        throw;
    else
        return it->second;
}
