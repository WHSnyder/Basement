#version 410 core

in vec2 texCoord;

uniform sampler2D tex;

layout (location = 0) out vec4 outColor;


void main(){
	
    outColor = vec4(vec3(texture(tex,texCoord).x), 1.0);
}
