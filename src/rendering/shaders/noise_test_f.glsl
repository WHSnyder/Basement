#version 410 core

in vec3 normal;
in vec2 texCoordsOut;
in vec3 position;

uniform sampler2D imageTex;
/*
uniform sampler2D shadowTex;

uniform mat4 shadowView;
uniform mat4 shadowProj;
*/

out vec4 outColor;


void main(){

	vec3 lightDir = normalize(vec3(0.0,-1.0,1.0));
	float mult = 0.0 - clamp(dot(normal,lightDir), -1.0, 0.0);

	/*vec4 shadowPos = shadowProj * shadowView * vec4(position,1.0);
	float shadowDepth = texture( shadowTex, .5 * (shadowPos.xy/shadowPos.w + vec2(1.0)) ).r;

	if (shadowPos.z/shadowPos.w - shadowDepth > .2){
		mult = shadowPos.z/shadowPos.w;
	}*/

    outColor = vec4(mult * texture(imageTex, texCoordsOut).bgr, 1.0);
}
