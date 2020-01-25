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

#ifndef obji
#define obji

class Obj {

	public:
		vec3 origin;
		virtual RayHit *intersect_ray(Ray& r)=0;
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Obj *objects[], Light *lights[])=0;
};


RayHit *intersect_scene(Obj *objs[], Ray& r, int *index){

	int i = 0, min_dist = 1000;
	RayHit *hit = nullptr, *cur = nullptr;
	
	while (objs[i] != nullptr){

		cur = objs[i] -> intersect_ray(r);

		if (cur != nullptr){
			if (cur -> distance < min_dist){
				delete hit;
				hit = cur;
				*index = i;
				min_dist = cur -> distance;
			}
			else {
				delete hit;
			} 
		}
		i++;
	}
	return hit;
}

#endif