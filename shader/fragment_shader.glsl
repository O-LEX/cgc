#version 410 core
out vec4 FragColor;

uniform vec3 rodColor;

void main(){
    FragColor = vec4(rodColor, 1.0);
}
