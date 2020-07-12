#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float exponent;
};
struct Spotlight {
    vec4 pos;
    vec3 dir;
    vec3 col;
    float cutoff;
    float exponent;
};

uniform mat4 m_proj;
uniform mat4 m_view;
uniform mat4 m_model;
uniform Material material;
uniform Spotlight spotlight;
uniform vec3 globalAmbient;
uniform vec3 eyePos;
uniform sampler2D chessboard;
uniform float isTextured;

in vec3 v_normal;
in vec3 v_pos;
in vec2 v_tex;

out vec4 color;

void main() {
    vec3 normal = normalize(v_normal);
    float spotExponent = clamp(spotlight.exponent, 0, 128);
    // Ambient
    vec3 ambient = globalAmbient * material.ambient;
    // Spotlight
    vec3 lDir = normalize(v_pos - spotlight.pos.xyz);
    float spotAngle = dot(spotlight.dir, lDir);
    float lit = float(acos(spotAngle) <= radians(spotlight.cutoff));
    float lIntensivity = lit * max(pow(spotAngle, spotExponent), 0);
    // Diffuse
    float diff = max(dot(normal, -lDir), 0.0); // -spotlight.dir
    vec3 diffuse = lIntensivity * spotlight.col * (diff * material.diffuse);
    // Specular
    vec3 rDir = reflect(-lDir, normal);
    float spec = pow(max(dot(rDir, normalize(v_pos - eyePos)), 0), material.exponent);
    vec3 specular = lIntensivity * spotlight.col * (spec * material.specular);
    // Final
    vec4 tex[2] = vec4[](vec4(1.0), texture(chessboard, v_tex));
    color = vec4(ambient + diffuse + specular, 1.0) * tex[int(isTextured)]; // isTextured * texture() + (1 - isTextured);
}