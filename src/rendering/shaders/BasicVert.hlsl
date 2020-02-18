#version 410 core

layout (location = 0) in vec3 inPosition;

out vec4 vertOutColor;

void main(){

    gl_Position = vec4(.5 * inPosition + vec3(0.0,0.0,2.0), 1.0);
    vertOutColor = vec4(1.0);
}
