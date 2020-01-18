#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
using namespace glm;
#endif 


#include "Ray.h"

#ifndef obj
#define obj

class Obj {

	public:
		vec3 origin;
		virtual vec3 intersect_ray(Ray r)=0;
};

#endif