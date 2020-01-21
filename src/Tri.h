#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include "Ray.h"
#include "Obj.h"

using namespace glm;

#ifndef tria
#define tria


class Tri : public Obj {

	public :
		vec3 v0,v1,v2;
		Tri(vec3 p0, vec3 p1, vec3 p2);
		virtual RayHit *intersect_ray(Ray r);
};
#endif