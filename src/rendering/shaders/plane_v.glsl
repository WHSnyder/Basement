#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 p;
uniform mat4 v;

out vec2 texCoord;

void main(){

	gl_Position = p * v * vec4(pos,1.0);
	texCoord = .5 * (pos.xz + vec2(1.0));
}