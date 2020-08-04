#ifndef __DEBUG_LAYOUT_H__
#define __DEBUG_LAYOUT_H__

#include "util/util.hpp"
#include "util/shader.hpp"
#include "util/font.hpp"

class DebugLayout {
private:
    Shader shader;
    Font *font;
    
    glm::vec3 pos;
    float yaw, pitch;
    bool groundFlag;
    bool flightmodFlag;
    bool isSelectedBlock;
    glm::ivec3 selectedBlock;
    WDir selectedFace;
    uint fps;

public:
    DebugLayout();
    ~DebugLayout();

    void show(const glm::mat4 &m_ortho, float width, float height) const;

    void setPos(const glm::vec3 &pos);
    void setPos(float x, float y, float z);
    void setView(float yaw, float pitch);
    void setGrounded(bool flag);
    void setFlightmoded(bool flag);
    void setSelectedBlock(const glm::ivec3 &block, WDir face, bool flag);
    void setFPS(uint fps);
};

#endif