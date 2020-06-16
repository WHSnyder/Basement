#version 430

layout(local_size_x = 16, local_size_y = 16) in;
uniform layout (binding = 0, rgba32f) writeonly image2D image;
layout(std430) buffer;
layout(binding = 1) buffer Output { float elements[]; } output_data;

void main() {

	ivec2 gid = ivec2(gl_GlobalInvocationID.xy);
	if (gid.x >= 384 || gid.y >= 384) return;
	int linear_index = 3 * (gid.y * 384 + gid.x);

	vec4 outCol = vec4(1.0);
  	outCol.x = output_data.elements[linear_index + 0];
  	outCol.y = output_data.elements[linear_index + 1];
  	outCol.z = output_data.elements[linear_index + 2];

	imageStore(image, gid.xy, outCol);
}