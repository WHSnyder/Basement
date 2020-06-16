#version 430 core

in vec2 texCoord;

layout(std430) buffer;
layout(binding = 1) buffer Output { float elements[]; } output_data;

/*uniform sampler2D imageTex;*/

out vec4 outColor;

void main(){

	ivec2 gid = ivec2(texCoord * ivec2(384,384));
	int linear_index = 3 * ((384 - gid.y) * 384 + gid.x);

	vec4 outCol = vec4(1.0);
  	outCol.x = output_data.elements[linear_index + 0];
  	outCol.y = output_data.elements[linear_index + 1];
  	outCol.z = output_data.elements[linear_index + 2];

    outColor = outCol;  /*vec4(texture(imageTex, texCoord).bgr, 1.0);*/
}