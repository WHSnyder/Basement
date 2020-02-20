#version 410 core

layout (location = 0) in vec3 inPosition;

uniform mat4 p;
uniform mat4 m;

out vec4 vertOutColor;

void main(){
	
	vec4 final = m * vec4(inPosition * .4, 1.0);
	final.z += -5.0;
    gl_Position = p * final;
    vertOutColor = vec4(1.0);
}
