#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 p, v, m;

out vec4 vertOutColor;

void main(){

	vec3 norm = mat3(m) * normal;

	vec3 lightDir = normalize(vec3(0.0,-1.0,1.0));
	float mult = clamp(0.0 - dot(norm,lightDir), 0.0, 1.0);
	

	gl_Position = p * v * m * vec4(pos,1.0);
    vertOutColor = vec4(mult * vec3(.2,0.7,0.2), 1.0);
}
