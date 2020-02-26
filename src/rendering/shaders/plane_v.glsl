#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

out vec2 texCoord;

void main(){

	gl_Position = p * v * m * vec4(pos,1.0);
	texCoord = .5 * (pos.xy + vec2(1.0));
}