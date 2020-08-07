#version 330 core
uniform sampler2D atlas;
in vec2 vtex;
out vec4 color;
void main() {
    color = texture(atlas, vtex);
}