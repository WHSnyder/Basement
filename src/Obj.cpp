#include "Obj.h"
#include <iostream>

using namespace std;

vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
}

RayHit *intersect_scene(Obj *objs[], Ray& r, int *index){

	int i = 0, min_dist = 1000;
	RayHit *hit = nullptr, *cur = nullptr;

	objs[3] = nullptr;
	
	while (objs[i] != nullptr){

		//cout << "On obj " << i << endl;

		cur = objs[i] -> intersect_ray(r);

		//cout << "Tested intersect" << endl;

		if (cur != nullptr){
			if (cur -> distance < min_dist){
				delete hit;
				hit = cur;
				*index = i;
				min_dist = cur -> distance;
			}
			else {
				delete cur;
			} 
		}
		i++;
	}
	return hit;
}