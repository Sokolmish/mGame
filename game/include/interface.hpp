#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "util/util.hpp"

class Interface {
public:
    Interface() = default;
    virtual ~Interface() = default;

    virtual size_t getReserveBuffSize() const = 0;
    virtual size_t getVertexCount() const = 0;
    virtual float* getLocBuff(uint width, uint height) const = 0;
    virtual float* getColBuff() const = 0;
};

class GameOverlay : public Interface {
private:
    uint vertexCount;
    float *buff;

    int selectedCell;

    size_t fillCrosshair(float *buff, uint width, uint height) const;
    size_t fillSidebar(float *buff, uint width, uint height) const;
public:
    GameOverlay();
    virtual ~GameOverlay();

    size_t getReserveBuffSize() const override;
    size_t getVertexCount() const override;
    float* getLocBuff(uint width, uint height) const override;
    float* getColBuff() const override;

    void selectSidebarCell(int cellnum);
};

#endif