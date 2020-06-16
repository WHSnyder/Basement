#version 430 

layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0) uniform sampler2D input_texture;
layout(std430) buffer;
layout(binding = 1) buffer Output { float elements[]; } output_data;
layout(location = 0) uniform int dim;

void main() {
  
  ivec2 gid = ivec2(gl_GlobalInvocationID.xy);
  if (gid.x >= dim || gid.y >= dim) return;
  
  vec3 pixel = texelFetch(input_texture, gid, 0).xyz;

  int linear_index = 3 * (gid.y * dim + gid.x);
  output_data.elements[linear_index + 0] = pixel.x;
  output_data.elements[linear_index + 1] = pixel.y;
  output_data.elements[linear_index + 2] = pixel.z;
}
