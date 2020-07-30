#version 330 core

uniform float cubeHalfSize;
uniform vec3 chunkOffset;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec3 cube_pos;
out vec2 vtex;

void main() {
    cube_pos = vec3(chunkOffset + pos) * (cubeHalfSize + cubeHalfSize);
    gl_Position = vec4(cube_pos, 1.0);
    vtex = tex;
}