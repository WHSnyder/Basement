#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
using namespace glm;
#endif 

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif


#include "Ray.h"

#ifndef obj
#define obj

class Obj {

	public:
		vec3 origin;
		virtual RayHit *intersect_ray(Ray& r)=0;
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat image, Obj *objects[], Light *lights[])=0;
};

#endif