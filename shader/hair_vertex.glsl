#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in float aThickness;
layout(location = 2) in float aTransparency;
layout(location = 3) in vec3 aColor;

out float vThickness;
out float vTransparency;
out vec3 vColor;

uniform bool useDefaultThickness;
uniform bool useDefaultTransparency;
uniform bool useDefaultColor;
uniform float defaultThickness;
uniform float defaultTransparency;
uniform vec3 defaultColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vThickness = useDefaultThickness ? defaultThickness : aThickness;
    vTransparency = useDefaultTransparency ? defaultTransparency : aTransparency;
    vColor = useDefaultColor ? defaultColor : aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
