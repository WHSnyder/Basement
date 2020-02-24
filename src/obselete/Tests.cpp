#include <cstdlib> 
#include <cstdio>
#include <vector>
#include <iostream>

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif

#include <opencv2/opencv.hpp> 

#include "Ray.h"
#include "Obj.h"
#include "Sphere.h"
#include "Plane.h"

#include <math.h>

using namespace std;
using namespace glm;



void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}


int main(){


	vec3 right = vec3(1.0,0.0,0.0);
	vec3 forward = vec3(0.0,-1.0,0.0);
	vec3 up = vec3(0.0,0.0,1.0);

	vec3 hit_pos = vec3(0.0,-4,0.0);

	Sphere s = Sphere(0,-3,2,1);
	Obj *os = &s;

	vec3 lightpos = vec3(0,-3,4);
	vec3 lightlook = s.origin;
	vec3 lightdir = normalize(lightlook - lightpos);

	Ray shadow = Ray(hit_pos, lightpos - hit_pos);
	RayHit *shadow_hit = os -> intersect_ray(shadow);

	if (shadow_hit != nullptr){
		printVec("Shadow hit",*(shadow_hit -> worldCoord));
		delete shadow_hit;

	}
	
	shadow_hit = nullptr;

	return 0;
}