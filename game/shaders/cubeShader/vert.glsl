#version 330 core

uniform float cubeHalfSize;

layout (location = 0) in vec3 pos;
layout (location = 1) in float block;

out vec3 cube_pos;

void main() {
    cube_pos = vec3(pos) * (cubeHalfSize + cubeHalfSize);
    gl_Position = vec4(cube_pos, 1.0);
}