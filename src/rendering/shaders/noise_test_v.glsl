#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 p;
uniform mat4 v;

uniform float dim;

uniform sampler2D tex;

out vec3 normal;

void main(){
	
	float inc = 1.0/dim;
	vec3 mult = 5.0 * vec3(3.0,1.0,3.0);	
	
	vec2 texCoord = (pos.xy + vec2(1.0)) * .5;
	float height = texture(tex,texCoord).r;

	vec3 position = mult * vec3(pos.x,height,pos.z);

	vec3 negx = vec3(pos.x - inc, pos.yz);
	vec3 negz = vec3(pos.xy, pos.z - inc);
	vec3 posx = vec3(pos.x + inc, pos.yz);
	vec3 posz = vec3(pos.xy, pos.z + inc);

	texCoord = (negx.xz + vec2(1.0)) * .5;
	negx.y = texture(tex,texCoord).r;
	negx *= mult; 

	texCoord = (negz.xz + vec2(1.0)) * .5;
	negz.y = texture(tex,texCoord).r;
	negz *= mult; 

	texCoord = (posx.xz + vec2(1.0)) * .5;
	posx.y = texture(tex,texCoord).r;
	posx *= mult; 

	texCoord = (posz.xz + vec2(1.0)) * .5;
	posz.y = texture(tex,texCoord).r;
	posz *= mult; 


	normal = 1.0 * normalize(cross(negz - posz, negx - posx));
	gl_Position = p * v * vec4(.25 * (posz + negz + posx + negx) - vec3(0.0,15.0,0.0),1.0);
}