#version 410 core

in vec3 normal;
in vec2 texCoordsOut;
in vec3 position;
in vec4 shadowCoord;

uniform sampler2D imageTex;
uniform sampler2D shadowTex;

out vec4 outColor;


void main(){

	vec3 lightDir = normalize(vec3(0.0,-1.0,-1.0));
	float mult = 0.0 - clamp(dot(normal,lightDir), -1.0, 0.0);
	
	if ( texture( shadowTex, shadowCoord.xy ).r  <  1.0){
    	mult = 0.2;
	}

    outColor = vec4(mult * texture(imageTex, texCoordsOut).bgr, 1.0);
}