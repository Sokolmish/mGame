#version 330 core

uniform mat4 m_proj_view;
uniform float cubeHalfSize;
uniform float textureW;
uniform float textureH;

layout (points) in;
layout (triangle_strip, max_vertices = 24) out; // TODO: triangles_strip

in vec2 vtex[];

out vec2 texCoord;
flat out int faceNum;

void emitQuad(int v1, int v2, int v3, int v4) {
    vec4 pos = gl_in[0].gl_Position;
    vec4 vertices[8] = vec4[](
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(-0, -0, +2), 1.0)), // l b n 0
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(+2, -0, +2), 1.0)), // r b n 1
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(+2, +2, +2), 1.0)), // r t n 2
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(-0, +2, +2), 1.0)), // l t n 3
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(-0, -0, -0), 1.0)), // l b f 4
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(+2, -0, -0), 1.0)), // r b f 5
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(+2, +2, -0), 1.0)), // r t f 6
        m_proj_view * (pos + vec4(cubeHalfSize * vec3(-0, +2, -0), 1.0))  // l t f 7
    );

    gl_Position = vertices[v1];
    texCoord = vec2(vtex[0].x, vtex[0].y);
    EmitVertex();
    gl_Position = vertices[v2];
    texCoord = vec2(vtex[0].x, vtex[0].y + textureH);
    EmitVertex();
    gl_Position = vertices[v3];
    texCoord = vec2(vtex[0].x + textureW, vtex[0].y);
    EmitVertex();
    gl_Position = vertices[v4];
    texCoord = vec2(vtex[0].x + textureW, vtex[0].y + textureH);
    EmitVertex();

    EndPrimitive();
}

void main() {
    faceNum = 0; // Top
    emitQuad(2, 6, 3, 7);
    faceNum = 1; // Bottom
    emitQuad(4, 5, 0, 1);
    faceNum = 3; // Left
    emitQuad(7, 4, 3, 0);
    faceNum = 2; // Right
    emitQuad(2, 1, 6, 5);
    faceNum = 4; // Forward
    emitQuad(6, 5, 7, 4);
    faceNum = 5; // Backward
    emitQuad(3, 0, 2, 1);
}