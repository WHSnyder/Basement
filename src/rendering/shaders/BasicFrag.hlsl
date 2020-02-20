#version 410 core

in vec4 vertOutColor;

layout (location = 0) out vec4 outColor;

void main(){
    outColor = vec4(vec3(gl_FragCoord.z), vertOutColor.a);
}
