#version 410 core

in vec3 normal;
in vec2 texCoordsOut;

uniform sampler2D imageTex;

layout (location = 0) out vec4 outColor;


void main(){

	vec3 lightDir = normalize(vec3(0.0,-1.0,1.0));
	float mult = 0.0 - clamp(dot(normal,lightDir), -1.0, 0.0);
    outColor = vec4(texture(imageTex, texCoordsOut).bgr * mult + vec3(.3,.3,.3), 1.0);
}
