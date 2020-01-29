#include "Scene.h"
#include <iostream>

using namespace std;

vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
}

RayHit *Scene::intersect_scene(Ray& r, int *index){

	int i = 0;
	float min_dist = 1000000000.0f;
	RayHit *hit = nullptr, *cur = nullptr;

	for (auto it = objects.begin(); it != objects.end(); it++) {

		cur = objects[i] -> intersect_ray(r);

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