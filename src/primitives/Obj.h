#ifndef glmi
#define glmi
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;
#endif 

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

#include "Ray.h"

#ifndef obji
#define obji

class Scene;

class Obj {

	public:
		vec3 origin;
		virtual RayHit *intersect_ray(Ray& r)=0;
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Scene *scene)=0;
};

#include "Scene.h"
#endif