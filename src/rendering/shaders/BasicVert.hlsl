#version 120 
attribute  vec3 in_Position; 

varying vec3 ex_Color; 
void main(void) { 
    gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0); 
    ex_Color = in_Color; 
}