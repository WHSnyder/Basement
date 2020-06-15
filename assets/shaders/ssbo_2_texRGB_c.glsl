#version 430 core

#extension GL_NV_gpushader5 : enable
#extension GL_EXT_shader_image_load_store : enable

layout(local_size_x = 16, local_size_y = 16) in;
uniform layout (binding = 0, rgba32f) writeonly image2D image;
/*layout( std140, binding = 0 ) buffer Buf {float elements[]; };*/

void main() {

	/*float mult = (1.0 + sin(1.0))/2.0;*/
  
	ivec2 gid = ivec2(gl_GlobalInvocationID.xy);
	if (gid.x >= 384 || gid.y >= 384) return;

	imageStore(image, gid.xy, vec4(0.0,0.0,0.0,1.0));
}