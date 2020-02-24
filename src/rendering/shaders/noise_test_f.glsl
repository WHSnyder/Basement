#version 410 core

in vec3 normal;

layout (location = 0) out vec4 outColor;


void main(){
	
	/*	
	float mult = 0.0 - clamp(dot(normal,lightDir), -1.0, 0.0);
    outColor = vec4(vec3(0.6,0.0,1.0) * mult, 1.0);
	*/

	vec3 lightDir = vec3(0.0,-1.0,0.0);

	outColor = vec4(normal + .00000001 * lightDir, 1.0);
}
