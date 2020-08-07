#include "../../include/util/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

#include "../../include/util/stb_image.hpp"

std::map<std::string, GLuint> Image::staticStorage;

Image::Image(const std::string &path) {
    int n;
    data = stbi_load(path.c_str(), &width, &height, &n, 4);
}

Image::~Image() {
    if (!isReleased) {
        stbi_image_free(data);
        isReleased = true;
    }
}

void Image::release() {
    if (!isReleased) {
        stbi_image_free(data);
        isReleased = true;
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
