#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 p;
uniform mat4 m;
uniform vec4 center;

out vec4 vertOutColor;

void main(){
	
	vec3 lightdir = normalize(vec3(-1.0,-4.0,-4.0));

	vec4 final = center + m * vec4(pos * .4, 1.0);
    gl_Position = p * final;

    float mult = clamp(-1.0 * dot(lightdir, (m * vec4(normal,1.0)).xyz ),0.0,1.0);

    vertOutColor = vec4(mult * vec3(1.0,0.0,0.6), 1.0);
}
