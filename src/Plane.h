#include "Ray.h"

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>
#include "Obj.h"


#ifndef planem
#define planem
using namespace glm;


class Plane : public Obj {

	public:
		vec3 xvec,yvec,zvec;
		float length,height;
		Plane(vec3 b1, vec3 b2, vec3 b3, vec3 b4);
		virtual RayHit *intersect_ray(Ray r);
};

#endif
