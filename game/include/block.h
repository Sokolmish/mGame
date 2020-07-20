#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdint.h>
#include "util.h"

class Block {
private:
    uint32_t data;
    // data:
    // 0-15 bits    Id
    // 16-18        Rotation about Y
    // 19-20        Rotation about X
    // 21-26        Lit (locked)
public:
    Block() {
        data = 0;
    }
    Block(short int Id) {
        data = Id;
    }

    void rotateY(WDir dir) {
        // data |= (dir & 0b11) << 16;
        throw "NotImplemented";
    }
    void rotateX(int dir) {
        // data |= (dir & 0b11) << 18;
        throw "NotImplemented";
    }

    WDir getYRotation() const {
        // return WDir(data & (0b11 << 16));
        throw "NotImplemented";
    }
    int getXRotation() const {
        // return WDir(data & (0b11 << 18));
        throw "NotImplemented";
    }

    uint16_t getId() const {
        return data & 0b1111111111111111; // 16 bit
    }

    uint32_t getData() const {
        return data;
    }
};

#endif