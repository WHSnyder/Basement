#version 410 core

in vec3 normal;
in vec2 texCoordsOut;
in vec3 position;
in vec4 shadowCoord;

uniform sampler2D imageTex;
uniform sampler2DShadow shadowTex;

out vec4 outColor;


void main(){

	vec3 lightDir = normalize(vec3(0.0,-1.0,-1.0));
	float mult = 0.0 - clamp(dot(normal,lightDir), -1.0, -0.1);
	
	/*
	if ( texture(shadowTex, shadowCoord.xy).r < shadowCoord.z){
    	mult = 0.3;
	}
	*/
	vec3 biasedShadowCoord = shadowCoord.xyz;
	biasedShadowCoord.z += .05;

    float shadowSample = texture(shadowTex, biasedShadowCoord);

    if (shadowSample < 0.5){
    	mult = .1;
    }

    outColor = vec4(mult * texture(imageTex, texCoordsOut).bgr + vec3(.1,.1,.1), 1.0);
}