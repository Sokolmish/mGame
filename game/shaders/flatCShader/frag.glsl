#version 330 core
in vec4 vcol;
out vec4 color;
void main() {
    color = vcol.rgba;
}