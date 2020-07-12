#version 330 core

uniform mat4 m_proj;
uniform mat4 m_view;
uniform mat4 m_model;
// uniform mat4 t_rot;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec2 texcord;

out vec3 v_normal;
out vec3 v_pos;
out vec2 v_tex;

void main() {
    // vec3 t_pos = vec3(t_rot * vec4(pos, 1));
    v_pos = vec3(m_model * vec4(pos, 1.0));
    gl_Position = m_proj * m_view * m_model * vec4(pos, 1.0);
    v_normal = in_norm; // multiply by normal's matrix
    v_tex = texcord;
}