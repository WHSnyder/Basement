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

	using Obj::intersect_ray;

	public:
		float radius;
		vec3 origin;
		Sphere(float x, float y, float z, float r);

		vec3 intersect_ray(Ray r) {

			vec3 to_center = origin - r.origin;

			float dotprod = dot(to_center, r.dir);
			float cos = dotprod / length(to_center);

			vec3 center_to_line = origin - dotprod * r.dir + r.origin;

			if (length(center_to_line) >= radius){
				return vec3(0.0,0.0,0.0);
			}

			return vec3(0.0,0.0,1.0);
		}
};

#endif