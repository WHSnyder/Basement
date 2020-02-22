#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

out vec4 vertOutColor;

void main(){
	
	vec3 lightdir = normalize(vec3(-1.0,-4.0,-4.0));
	vec3 norm = (m * vec4(normal,1.0)).xyz;

	float mult = clamp(1.0 - dot(norm,lightdir),0.0,1.0);
	
	gl_Position = p * v * m * vec4(pos,1.0);
    vertOutColor = vec4(mult * vec3(1.0,0.0,0.6), 1.0);
}
