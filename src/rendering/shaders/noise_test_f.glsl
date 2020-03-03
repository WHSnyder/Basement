#version 410 core

in vec3 normal;
in vec2 texCoordsOut;
in vec3 position;

uniform sampler2D imageTex;
uniform sampler2D shadowTex;

uniform mat4 shadowView, shadowProj, shadowBias;

out vec4 outColor;



void main(){

	/*vec3 lightDir = normalize(vec3(0.0,-1.0,1.0));
	float mult = 0.0 - clamp(dot(normal,lightDir), -1.0, 0.0);

	vec4 shadowTexCoord = shadowBias * shadowProj * shadowView * vec4(position,1.0);
	
	float shadowDepth = texture(shadowTex, shadowTexCoord.xy).r;

	if (shadowDepth < shadowTexCoord.z){
    	mult = 0.2;
	}*/

	float mult = texture(shadowTex, position.xz/30.0 + vec2(1.0)).r;

    outColor = vec4(mult * texture(imageTex, texCoordsOut).bgr, 1.0);
}