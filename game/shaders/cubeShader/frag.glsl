#version 330 core

uniform sampler2D cubeTexture;

in vec2 texCoord;
flat in int faceNum;

out vec4 color;

void main() {
    color = texture(cubeTexture, texCoord);
}