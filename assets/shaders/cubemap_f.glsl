#version 410 core

in vec3 texCoords;

uniform samplerCube imageTex;

out vec4 fragColor;

void main(){

    fragColor = texture(imageTex, texCoords).bgra;
}