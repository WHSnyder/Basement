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

#ifndef scenei
#define scenei


class Scene {

	public:
		std::vector<Obj *> objects; 
		std::vector<Light *> lights; 

		RayHit *intersect_scene(Ray& r, int *index);

		void add_light(Light *l){
			lights.push_back(l);
		}

		void add_object(Obj *o){
			objects.push_back(o);
		}
};

vec3 reflect(vec3 normal, vec3 direction);
#endif