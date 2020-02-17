#version 410 core

in vec3 position;
out vec4 color;

void main(void){

    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    color = vec4(1.0);
}