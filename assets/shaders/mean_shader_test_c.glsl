#version 430
layout(local_size_x = 16, local_size_y = 16, local_size_z = 4) in;
layout(std430) buffer;
precision mediump float;
#define Vec4FromHalf(v) vec4(unpackHalf2x16(v.x), unpackHalf2x16(v.y))
#define Vec4ToHalf(v) uvec2(packHalf2x16(v.xy), packHalf2x16(v.zw))
layout(binding = 1) writeonly buffer B1 { uvec2 data[]; } output_data_0;
layout(binding = 0) readonly buffer B0 { uvec2 data[]; } input_data_0;


shared highp vec4 sh_mem[1024];

void main() {

    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);
    highp vec4 value_0 = vec4(0);

    {
        highp vec4 sum = vec4(0.0);
        highp float inputSize = float(384 * 384);

        const int sizeY = int(gl_WorkGroupSize.y);
        const int sizeX = int(gl_WorkGroupSize.x);
        ivec3 localID = ivec3(gl_LocalInvocationID);

        int workGridSize = sizeX * sizeY;
        int taskSize = int(ceil(inputSize/float(workGridSize)));

        int gridID = localID.x * sizeY + localID.y;
        int z_offset = localID.z * int(inputSize);

        int startIndex = gridID * taskSize;

        for (int i = startIndex; i < startIndex + taskSize; i++) {
            sum += i < inputSize ? Vec4FromHalf(input_data_0.data[i + z_offset + 384 * ( 0 + 384 * ( 0))]) : vec4(0.0);
        }

        sh_mem[gridID + z_offset] = sum;

        memoryBarrierShared();
        barrier();

        if (gid.x >= 1 || gid.y >= 1 || gid.z >= 4){
            return;
        }

        sum = vec4(0.0);
        z_offset = workGridSize * localID.z;
        
        for (int i = 0; i < workGridSize; i++){
          sum += sh_mem[i + z_offset];  
        }

        value_0 = sum / inputSize;
    }
	
	output_data_0.data[gid.x + 1 * ( gid.y + 1 * ( gid.z))] = Vec4ToHalf(value_0);
}