#include "../include/image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

#include "../include/stb_image.h"

Image::Image(const std::string &path) {
    int n;
    data = stbi_load(path.c_str(), &width, &height, &n, 3);
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

uint8_t* Image::getData() {
    return data;
}

int Image::getW() {
    return width;
}

int Image::getH() {
    return height;
}