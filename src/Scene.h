#ifndef scenei
#define scenei

#ifndef glmi
#define glmi
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#endif 

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

#include "Ray.h"
#include "primitives/Obj.h"
#include "CSG.h"

using namespace glm;

class CSG;

class Scene {

	public:
		std::vector<Obj *> objects; 
		std::vector<Light *> lights; 
		std::vector<CSG *> csgs;

		RayHit *intersect_scene(Ray& r, int *index);

		void add_light(Light *l){
			lights.push_back(l);
		}

		void add_object(Obj *o){
			objects.push_back(o);
		}

		void add_csg(CSG *c){
			csgs.push_back(c);
		}
};

vec3 reflect(vec3 normal, vec3 direction);
#endif