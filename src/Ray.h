#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#ifndef rayi
#define rayi
using namespace glm;


class Light {

	public:
		vec3 location;
		vec3 direction;
		vec3 color;

	Light(vec3 l, vec3 d, vec3 c){
		location = l;
		direction = normalize(d);
		color = c;
	}
};

class Ray {

	public:
		vec3 dir;
		vec3 origin;

		Ray(vec3 o, vec3 d){
			dir = normalize(d);
			origin = o;
		}		
};

class RayHit {

	public:
		vec3 *worldCoord;
		vec2 *uv;
		vec3 *normal;
		Ray *ray;
		float distance;

		RayHit(vec3 *w, vec2 *_uv, vec3 *n, float d, Ray *r){
			worldCoord = w;
			uv = _uv;
			normal = n;
			distance = d;
			ray = r;
		}

		~RayHit(){
			delete worldCoord;
			delete uv;
			delete normal;
		}
};
#endif