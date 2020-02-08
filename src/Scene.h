#ifndef scenei
#define scenei

#include "Ray.h"
#include <vector> 
#include <primitives/Obj.h>

using namespace glm;

#define sphere_mass 1
#define plane_mass 100000 


class CSG;


class View {
	public:

		vec3 forward, up, right, pos;

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

		Plane *test_plane;
		Sphere *test_sphere;
		Sphere *static_sphere;

		float timestep = 0.0f, incstep = 1.0f/30.0f;

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

		void update_physics();

		/*
		~Scene(){
			delete view;
			int i = 0;
			for (auto it = csgs.begin(); it != csgs.end(); it++) {
				delete csgs[i];
				i++;
			}
			i = 0;
			for (auto it = lights.begin(); it != lights.end(); it++) {
				delete lights[i];
				i++;
			}
		}*/
};

vec3 reflect(vec3 normal, vec3 direction);
#endif