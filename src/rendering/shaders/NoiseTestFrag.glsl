#version 410 core

in vec2 texCoord;

uniform sampler2D tex;

layout (location = 0) out vec4 outColor;

void main(){

	vec4 smp = vec4(texture(tex, texCoord).r);
	smp.a = 1.0;
    outColor = smp;
}
