#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 p, v;

uniform float dim;
uniform vec3 mult;

uniform sampler2D dataTex;

out vec3 normal;
out vec2 texCoordsOut;

void main(){
	
	float inc = 1.0/dim;
	float scale = .5;
	
	vec2 texCoord = (pos.xz + vec2(1.0)) * scale;
	float height = texture(dataTex,texCoord).r;

	vec3 position = mult * vec3(pos.x,height,pos.z);

	vec3 negx = vec3(pos.x - inc, pos.yz);
	vec3 negz = vec3(pos.xy, pos.z - inc);
	vec3 posx = vec3(pos.x + inc, pos.yz);
	vec3 posz = vec3(pos.xy, pos.z + inc);

	texCoord = (negx.xz + vec2(1.0)) * scale;
	negx.y = texture(dataTex,texCoord).r;
	negx *= mult; 

	texCoord = (negz.xz + vec2(1.0)) * scale;
	negz.y = texture(dataTex,texCoord).r;
	negz *= mult; 

	texCoord = (posx.xz + vec2(1.0)) * scale;
	posx.y = texture(dataTex,texCoord).r;
	posx *= mult; 

	texCoord = (posz.xz + vec2(1.0)) * scale;
	posz.y = texture(dataTex,texCoord).r;
	posz *= mult; 


	normal = normalize(cross(negz - posz, negx - posx));
	gl_Position = p * v * vec4(position, 1.0);

	texCoordsOut = position.xz/10.0;
}