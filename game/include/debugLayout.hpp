#ifndef __DEBUG_LAYOUT_H__
#define __DEBUG_LAYOUT_H__

#include <glm/vec3.hpp>
#include "shader.hpp"
#include "font.hpp"

class DebugLayout {
private:
    Shader shader;
    Font font;
    
    glm::vec3 pos;
    float yaw, pitch;
    bool groundFlag;
    bool flightmodFlag;

public:
    DebugLayout();
    void show(float width, float height) const;

    void setPos(const glm::vec3 &pos);
    void setPos(float x, float y, float z);
    void setView(float yaw, float pitch);
    void setGrounded(bool flag);
    void setFlightmoded(bool flag);
};

#endif