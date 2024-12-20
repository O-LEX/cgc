#version 410 core
layout(location = 0) in vec3 aPos;

out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vColor = vec3(1.0, 0.0, 0.0);
}