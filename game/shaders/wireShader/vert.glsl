#version 330 core

uniform mat4 m_proj_view;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 vcol;

void main() {
    gl_Position = m_proj_view * vec4(pos, 1.0);
    vcol = col;
}