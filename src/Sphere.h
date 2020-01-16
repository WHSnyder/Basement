#include "Ray.h"

#ifndef glm
#define glm
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
using namespace glm;
#endif 

#include <math.h>
#include "Obj.h"


#ifndef sphere
#define sphere


class Sphere : public Obj {

	using Obj::intersect_ray;

	public:
		float radius;
		Sphere(float x, float y, float z, float r);

		vec3 intersect_ray(Ray r) override {

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