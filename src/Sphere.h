#include "Ray.h"

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>
#include "Obj.h"


#ifndef sphere
#define sphere
using namespace glm;


class Sphere : public Obj {

	public:
		float radius;
		Sphere(float x, float y, float z, float r);
		virtual RayHit *intersect_ray(Ray& r);
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat image, Obj *objects[], Light *lights[]);
};

#endif