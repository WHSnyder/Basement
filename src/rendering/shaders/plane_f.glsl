#version 410 core

in vec2 texCoord;

uniform sampler2D dataTex;

layout (location = 0) out vec4 outColor;


void main(){
	
    outColor = vec4(vec3(texture(dataTex, texCoord).x), 1.0);
}
