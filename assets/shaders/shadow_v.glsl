#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 p, v, m;

void main(){
	gl_Position = p * v * m * vec4(pos,1.0);
}