#ifndef scenei
#define scenei

#include "CSG.h"


using namespace glm;


class CSG;


class View {
	public:
		vec3 forward;
		vec3 up;
		vec3 right;
		vec3 pos;

		View(vec3 f, vec3 u, vec3 r, vec3 p){
			forward=f;
			up=u;
			right=r;
			pos = p;
		}
};


class Scene {

	public:
		std::vector<Light *> lights; 
		std::vector<CSG *> csgs;
		View *view;

		RayHit *intersect_scene(Ray& r, int *index);

		void add_light(Light *l){
			lights.push_back(l);
		}

		void add_csg(CSG *c){
			csgs.push_back(c);
		}

		Scene(View *v){
			view=v;
		}
};

vec3 reflect(vec3 normal, vec3 direction);
#endif