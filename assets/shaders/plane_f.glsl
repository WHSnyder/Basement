#version 410 core

in vec2 texCoord;

uniform sampler2D imageTex;

out vec4 outColor;

void main(){
	float d = texture(imageTex, texCoord).r;
    outColor = vec4(d*d, d*d, d*d, 1.0);
}