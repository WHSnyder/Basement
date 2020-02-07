#include "Scene.h"
#include "CSG.h"
#include <iostream>

#define GRAV vec3(0,0,-4.2);

using namespace std;


vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
}


vec3 box_sphere





void Scene::update_physics(){

	vec3 center_to_plane = test_plane -> origin - test_sphere -> origin;
	float dist_to_plane = -1.0f * dot(test_plane -> zvec, center_to_plane);

	if (dist_to_plane < test_sphere -> radius){
		//cout << "Hit: " << dist_to_plane << endl; 
		test_sphere -> vel = reflect(test_plane -> zvec, test_sphere -> vel);
	}

	test_sphere -> vel += incstep * GRAV;
	test_sphere -> origin += test_sphere -> vel * incstep;
}


RayHit *Scene::intersect_scene(Ray& r, int *index){

	int i = 0;
	float min_dist = 1000000000.0f;
	RayHit *hit = nullptr, *cur = nullptr;

	for (auto it = csgs.begin(); it != csgs.end(); it++) {

		cur = csgs[i] -> intersect_ray(r);

		if (cur != nullptr){
			
			if (cur -> ent_distance < min_dist){

				delete hit;
				hit = cur;
				*index = i;
				min_dist = cur -> ent_distance;
			}
			else {

				delete cur;
			} 
		}

		i++;
	}
	return hit;
}