#version 410 core

in vec2 texCoord;

uniform sampler2D imageTex;

out vec4 outColor;

void main(){
    outColor = vec4(texture(imageTex, texCoord).rgb, 1.0);
}