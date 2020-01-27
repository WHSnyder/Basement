#include "Obj.h"
#include <iostream>

using namespace std;

vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
}

RayHit *intersect_scene(Obj *objs[], Ray& r, int *index){

	int i = 0, min_dist = 1000;
	RayHit *hit = nullptr, *cur = nullptr;

	objs[4] = nullptr;
	
	while (objs[i] != nullptr){

		cur = objs[i] -> intersect_ray(r);

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