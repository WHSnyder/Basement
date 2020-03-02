#version 410 core

layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 p, v;

void main(){
    TexCoords = pos;
    gl_Position = p * v * vec4(pos, 1.0);
} 