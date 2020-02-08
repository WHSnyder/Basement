#include "Scene.h"
#include "CSG.h"
#include <iostream>

#define GRAV vec3(0,0,-9.8);

using namespace std;


vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
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

Contact *sphere_sphere(Sphere *s0, Sphere *s1, int mode){

	vec3 to_center = s1 -> origin - s0 -> origin, new_orig, normal;
	float dist,l,r;
	Contact *result = nullptr;

	dist = length(to_center);

	//Test for normal collision
	if (mode == 1){

		if (dist < s1 -> radius + s0 -> radius){
		
			//length(curr_orig + r * t - origin) = s0rad + s1rad;    r*t = r + origin - curr
			
			//r = s1 -> radius;
			//l = -1.0 * abs( (1 + dot(r,s1 -> origin - s0 -> origin)/r*r) );
			//new_orig = s0 -> origin += normalize(s0 -> vel) * 
			//pt = new vec3(s0 -> origin + )

			normal = -1.0f * normalize(to_center);
			new_orig = s0 -> origin + (s0 -> radius - (dist - s1 -> radius)) * normal;

			result = new Contact(normal, new_orig);
		}
	}

	//Test for inverse collision
	else {

		if (dist >  s1 -> radius - s0 -> radius && dot(to_center,s0 -> vel) < 0){

			normal = normalize(to_center);
			new_orig = s0 -> origin + (dist - s1 -> radius) * normal;

			result = new Contact(normal, new_orig);
		}
	}

	return result;
}




void Scene::update_physics(){

	vec3 center_to_plane = test_plane -> origin - test_sphere -> origin;
	float move_back,pen_dist,vel_mag,dist_to_plane = -1.0f * dot(test_plane -> zvec, center_to_plane);
	Contact *ct;

	if (dist_to_plane < test_sphere -> radius){
		
		//cout << "Hit: " << dist_to_plane << endl; 
		//vel_mag = length(test_sphere -> vel);
		//move_back =  dot(test_sphere -> vel / vel_mag,-1.0 * test_plane -> zvec)
		//pen_dist =

		test_sphere -> origin += (test_sphere -> radius - dist_to_plane) * test_plane -> zvec; 
		test_sphere -> vel = .9f * reflect(test_plane -> zvec, test_sphere -> vel);
	}

	ct = sphere_sphere(test_sphere,static_sphere,-1);

	if (ct != nullptr){

		test_sphere -> origin = ct -> point; 
		test_sphere -> vel = .9f * reflect(ct -> normal, test_sphere -> vel );

		delete ct;
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