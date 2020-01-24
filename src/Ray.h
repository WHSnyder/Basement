#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#ifndef ray
#define ray
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
}

class RayHit {

	public:
		vec3 *worldCoord;
		vec2 *uv;
		vec3 *normal;
		float distance;

		RayHit(vec3 *w, vec2 *_uv, vec3 *n, float d){
			worldCoord = w;
			uv = _uv;
			normal = n;
			distance = d;
		}

		~RayHit(){
			delete worldCoord;
			delete uv;
			delete normal;
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


RayHit *intersect_scene(Obj *objs[], Ray& r){

	int i = 0, min_dist = 1000;
	RayHit *hit = nullptr, *cur = nullptr;
	
	while (objs[i] != nullptr){

		cur = objs[i] -> intersect_ray(r);

		if (cur != nullptr){
			if (cur -> distance < min_dist){
				delete hit;
				hit = cur;
			}
			else {
				delete hit;
			} 
		}
		i++;
	}
	return hit;
}

vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
}
#endif