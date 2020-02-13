#include "Scene.h"
#include "CSG.h"
#include <iostream>

#define GRAV vec3(0,0,-3.0);

using namespace std;


inline vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
}

inline void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}

/*
int box_sphere(Sphere *sphere, Cube *cube, int mode){

	vec3 to_cube = cube -> origin - sphere -> origin;
	int mult = -1 + 2*mode;
	
	float xdist,ydist,zdist;

	xdist = dot(to_cube,rightdir);
	ydist = dot(to_cube,forwarddir);
	zdist = dot(to_cube,updir);

	if (xdist <= ydist && xdist <= zdist){

	}
	else if (ydist <= xdist)

	if (xdist - rightdim < sphere -> radius)
}
*/


void Scene::update_physics(){

	int i = 0;
	Contact *cur = nullptr;

	for (auto it = csgs.begin(); it != csgs.end(); it++) {

		cur = csgs[i] -> collide_sphere(test_sphere, 1);

		if (cur != nullptr){

			if (!isnan(cur -> point[0])){
				
				test_sphere -> origin = cur -> point; 

				if (dot(test_sphere -> vel, cur -> normal) < 0){
		    		
		    		test_sphere -> vel = .8f * reflect(cur -> normal, test_sphere -> vel);
		    	}

		    	delete cur;
		    	break;
			}

			delete cur;
		}

		i++;
	}

	test_sphere -> vel += incstep * GRAV;
	test_sphere -> origin += test_sphere -> vel * incstep;
}


RayHit *Scene::intersect_scene(Ray& r){

	int i = 0;
	float min_dist = 1000000000.0f;
	RayHit *hit = nullptr, *cur = nullptr;

	for (auto it = csgs.begin(); it != csgs.end(); it++) {

		cur = csgs[i] -> intersect_ray(r);

		if (cur != nullptr){
			
			if (cur -> ent_distance < min_dist){

				delete hit;
				hit = cur;
				min_dist = cur -> ent_distance;
			}

			else delete cur;
		}

		i++;
	}

	return hit;
}