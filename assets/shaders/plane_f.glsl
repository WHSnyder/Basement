#version 430 core

in vec2 texCoord;

uniform sampler2D imageTex;

out vec4 outColor;

void main(){
    outColor = texture(imageTex,texCoord);
}