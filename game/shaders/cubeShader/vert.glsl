#version 330 core

layout (location = 0) in vec3 pos;

out vec3 cube_pos;

void main() {
    cube_pos = pos;
    gl_Position = vec4(pos, 1.0);
}