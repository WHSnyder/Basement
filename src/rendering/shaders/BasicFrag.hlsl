#version 120 
precision highp float; 

varying vec3 ex_Color; 

void main(void) { 
    gl_FragColor = vec4(ex_Color,1.0); 
} 