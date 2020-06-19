#version 430 core

in vec2 texCoord;

//layout(std430) buffer;
//layout(binding = 1) buffer Output { float elements[]; } output_data;
//uniform layout (binding = 0, rgba32f) readonly image2D image;

uniform sampler2D imageTex;


out vec4 outColor;

void main(){

	/*int dim = 384;
	ivec2 gid = ivec2(texCoord * ivec2(dim));

	int linear_index = 3 * ((dim - gid.y) * dim + dim - gid.x);

	if (linear_index >= dim * dim * 3){
		return;
	}

	vec4 outCol = vec4(1.0);
  	outCol.x = output_data.elements[linear_index + 0];
  	outCol.y = output_data.elements[linear_index + 1];
  	outCol.z = output_data.elements[linear_index + 2];
  	*/

    outColor = texture(imageTex,texCoord);   //outCol;
}