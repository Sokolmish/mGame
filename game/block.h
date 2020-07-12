#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdint.h>

enum WorldDirection { NORTH, SOUTH, EAST, WEST };

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

    void rotateY(WorldDirection dir) {
        data |= (dir & 0b11) << 16;
    }
    void rotateX(int dir) {
        data |= (dir & 0b11) << 18;
    }

    WorldDirection getYRotation() const {
        return WorldDirection(data & (0b11 << 16));
    }
    int getXRotation() const {
        return WorldDirection(data & (0b11 << 18));
    }

    uint32_t getData() const {
        return data;
    }
};

#endif