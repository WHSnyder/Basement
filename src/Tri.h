#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include "Ray.h"
#include "Obj.h"

using namespace glm

#ifndef tria
#define tria




class Tri : public Obj {

	public :
		vec3 v1,v2,v3;
		
		Tri(vec3 _p1, vec3 _p2, vec3 _p3){
			v1 = _p1;
			v2 = _p2;
			v3 = _p3;
			origin = (v1 + v2 + v3)/3;
		}
		virtual RayHit *intersect_ray(Ray r);


};

#endif

