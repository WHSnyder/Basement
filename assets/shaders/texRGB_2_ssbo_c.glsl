#version 430 

layout(local_size_x = 16, local_size_y = 16) in;
layout(std430) buffer;
layout(binding = 0) buffer Output { float elements[]; } output_data;

uniform sampler2D imageTex;

void main() {

	int dim = 384;
	ivec2 gid = ivec2(gl_GlobalInvocationID.xy);
	if (gid.x >= dim || gid.y >= dim) return;

	vec3 pixel = texelFetch(imageTex, gid, 0).xyz;

	int linear_index = 3 * (gid.y * dim + gid.x);
	output_data.elements[linear_index + 0] = pixel.r;
	output_data.elements[linear_index + 1] = pixel.g;
	output_data.elements[linear_index + 2] = pixel.b;
}