#include "Obj.h"


#ifndef cubem
#define cubem
using namespace glm;


class Cube : public Obj {

	public:
		vec3 min,max;
		Cube(vec3 _lb, vec3 _ub);
		virtual RayHit *intersect_ray(Ray& r);
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Obj *objects[], Light *lights[]);
};

#endif