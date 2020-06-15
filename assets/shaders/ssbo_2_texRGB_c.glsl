#version 430

layout(local_size_x = 16, local_size_y = 16) in;
uniform layout (binding = 0, rgba32f) writeonly image2D image;

layout(std430) buffer;
layout(binding = 0) buffer Output { float elements[]; } output_data;
void main() {

	/*float mult = (1.0 + sin(1.0))/2.0;*/
  
	ivec2 gid = ivec2(gl_GlobalInvocationID.xy);
	if (gid.x >= 384 || gid.y >= 384) return;

	int linear_index = 3 * (gid.y * 384 + gid.x);
  	float x = output_data.elements[linear_index + 0];
  	float y = output_data.elements[linear_index + 1];
  	float z = output_data.elements[linear_index + 2];

	imageStore(image, gid.xy, vec4(x,y,z,1.0));
}