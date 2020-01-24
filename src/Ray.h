#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#ifndef ray
#define ray
using namespace glm;

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
#endif