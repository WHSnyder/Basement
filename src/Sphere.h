#include "Ray.h"

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>


#ifndef sphere
#define sphere
using namespace glm;


class Sphere {

	public:
		float radius;
		vec3 origin;
		Sphere(float x, float y, float z, float r);
		vec3 intersect_ray(Ray r);
};

#endif