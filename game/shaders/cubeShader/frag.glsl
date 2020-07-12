#version 330 core

uniform sampler2D cubeTexture;

in vec2 texCoord;
flat in int faceNum;

out vec4 color;

void main() {
    // vec4 colors[6] = vec4[](
    //     vec4(1.0, 0.0, 0.0, 1.0),
    //     vec4(0.5, 0.0, 0.0, 1.0),
    //     vec4(0.0, 1.0, 0.0, 1.0),
    //     vec4(0.0, 0.5, 0.0, 1.0),
    //     vec4(0.0, 0.0, 1.0, 1.0),
    //     vec4(0.0, 0.0, 0.5, 1.0)
    // );
    // color = colors[int(faceNum)];

    color = texture(cubeTexture, texCoord);
}