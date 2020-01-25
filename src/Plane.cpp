#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

using namespace glm;

#include "Plane.h"

#include <iostream>
using namespace std;

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

cv::Vec3b Plane::shade(RayHit *rhit, cv::Mat *img, Obj *objects[], Light *lights[]){

	vec2 uv = *rhit -> uv;
	vec3 col = lights[0]->color;

	int i = -1;
	int u = (int) (img->rows - 1) * uv.x;
	int v = (int) (img->cols - 1 )* uv.y;

	vec3 hit_pos = *rhit -> worldCoord;

	Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
	RayHit *shadow_hit = intersect_scene(objects, shadow, &i);

	float dotprod = -1.0f * dot(*rhit -> normal,lights[0]->direction);
	dotprod = dotprod < .2 ? .2 : dotprod;

	if (shadow_hit != nullptr) dotprod = .2f;

	delete shadow_hit;

	return dotprod * cv::Vec3b(col.x,col.y,col.z).mul( img->at<cv::Vec3b>(u,v) )/255; 
}


Plane::Plane(vec3 _b1, vec3 _b2, vec3 _b3, vec3 _b4){

	origin = (_b1 + _b2 + _b3 + _b4)/4.0f;

	vec3 _xvec = (_b1 + _b2)/2.0f - origin;
	vec3 _yvec = (_b2 + _b3)/2.0f - origin;

	this->length = glm::length(_yvec);
	this->height = glm::length(_xvec);

	xvec = normalize(_xvec);
	yvec = normalize(_yvec);
	zvec = cross(xvec,yvec);

	if (zvec.z < 0){
		zvec *= -1.0f;
	}
}


RayHit *Plane::intersect_ray(Ray& r) {

    float denom = dot(zvec,r.dir); 

    if (denom < -0.001) { 
    	//cout << r.origin.x << " " << r.origin.y << " " << r.origin.z << endl;

        float t = (dot(zvec,origin) - dot(zvec,r.origin))/denom;

        if (t < .001) return nullptr; 

        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - origin;

        float u = dot(fromOrg, xvec) / this->height;
        float v = dot(fromOrg, yvec) / this->length;

        if (abs(u) > 1.0 || abs(v) > 1.0){
        	return nullptr; 	
        }

        RayHit *planehit = new RayHit(hit, new vec2((1+u)/2,(1+v)/2), new vec3(zvec), t, &r);
        return planehit;
    } 
    return nullptr; 
} 