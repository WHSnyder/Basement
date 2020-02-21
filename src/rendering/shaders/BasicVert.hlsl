#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 p;
uniform mat4 m;

out vec4 vertOutColor;

void main(){
	
	vec3 lightpos = vec3(4.0,4.0,-4.0);

	vec4 final = m * vec4(pos * .4, 1.0);
	final.z += -5.0;
    gl_Position = p * final;

    float mult = clamp(-1.0 * dot(normalize(lightpos - pos), normal),0.0,1.0);

    vertOutColor = vec4(mult * vec3(1.0,0.0,0.6), 1.0);
}
