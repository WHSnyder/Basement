using namespace glm;

#include "Cube.h"

#include <iostream>
using namespace std;


vec3 xbase = vec3(1,0,0);
vec3 ybase = vec3(0,1,0);
vec3 zbase = vec3(0,0,1);


cv::Vec3b Cube::shade(RayHit *rhit, cv::Mat *img, Obj *objects[], Light *lights[]){

	vec3 col = lights[0]->color;

	int i = -1;

	vec3 hit_pos = *rhit -> worldCoord;

	Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
	RayHit *shadow_hit = intersect_scene(objects, shadow, &i);

	float dotprod = -1.0f * dot(*rhit -> normal,lights[0]->direction);
	dotprod = dotprod < .2 ? .2 : dotprod;

	dotprod = 1.0f;
	if (shadow_hit != nullptr) dotprod = .2f;

	delete shadow_hit;

	return dotprod * cv::Vec3b(30,200,30);
}


Cube::Cube(vec3 lb, vec3 ub){

	origin = (lb + ub)/2.0f;
	min = lb;
	max = ub;
}

RayHit *Cube::intersect_ray(Ray &r) {

    float tmin,tmax,tymin,tymax,tzmin,tzmax,temp,xlfip=1.0f,ylfip=1.0f,zflip=1.0f;
    vec3 ent,exit,ent_normal,exit_normal,min_norm,max_norm; 
    RayHit *hit;

    tmin = 100000000;
    tmax = -100000000;

    if (r.dir.x)

    tmin = (min.x - r.origin.x) / r.dir.x; 
    tmax = (max.x - r.origin.x) / r.dir.x;

    if (tmin > tmax){
    	temp = tmax;
    	tmax = tmin;
    	tmin = temp;

    	xlfip = -1.0f
    }

    min_norm = xflip * xbase;
    max_norm = xflip * -1.0f * xbase;
 
    tymin = (min.y - r.origin.y) / r.dir.y; 
    tymax = (max.y - r.origin.y) / r.dir.y;

    if (tymin > tymax){
    	temp = tymax;
    	tymax = tymin;
    	tymin = temp;

    	yflip = -1.0f;
    } 
 
    if ((tmin > tymax) || (tymin > tmax)){ 
        return nullptr; 
    }

    if (tymin > tmin){ 
        tmin = tymin;
        min_norm = yflip * -1.0f * ybase;
    }

    if (tymax < tmax){ 
        tmax = tymax; 
        max_norm = yflip * ybase;
    }
 
    tzmin = (min.z - r.origin.z) / r.dir.z; 
    tzmax = (max.z - r.origin.z) / r.dir.z;

    if (tzmin > tzmax){
    	temp = tzmax;
    	tzmax = tzmin;
    	tzmin = temp;

    	zflip = -1.0f;
    }  
 
    if ((tmin > tzmax) || (tzmin > tmax)){
        return nullptr; 
    }

    if (tzmin > tmin){ 
        tmin = tzmin; 
        min_norm = zflip * -1.0f * zbase;
    }

    if (tzmax < tmax){
        tmax = tzmax;
        max_norm = yflip * ybase; 
    }

    if (tmin < 0){
    	return nullptr;
    }

    ent = new vec3(r.origin + tmin * r.dir);
    exit = new vec3(r.origin + tmax * r.dir);





 
    return true; 
} 