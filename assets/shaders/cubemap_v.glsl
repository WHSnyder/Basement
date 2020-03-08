#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 p, v;

out vec3 texCoords;

void main(){
    texCoords = pos;
    gl_Position = p * vec4(mat3(v) * 1000.0 * pos, 1.0);
} 