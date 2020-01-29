#include "Plane.h"
#include <iostream>

using namespace glm;
using namespace std;


cv::Vec3b Plane::shade(RayHit *rhit, cv::Mat *img, Scene *scene){

	vec3 hit_pos = *rhit -> entrance;
    vec3 fromOrg = hit_pos - origin;
    std::vector<Light *> lights = scene -> lights;

	int i = -1;
	int row = (int) (img->rows - 1) * ((dot(fromOrg, xvec) / this->height) + 1.0f)/2.0f;
	int col = (int) (img->cols - 1) * ((dot(fromOrg, yvec) / this->length) + 1.0f)/2.0f;

	Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
	RayHit *shadow_hit = scene -> intersect_scene(shadow, &i);

	//float dotprod = -1.0f * dot(*rhit -> ent_normal, lights[0]->direction);

	float dotprod = 1.0f;

	if (shadow_hit != nullptr) dotprod = .2f;

	delete shadow_hit;

	return dotprod * img->at<cv::Vec3b>(row, col);
}


Plane::Plane(vec3 _b1, vec3 _b2, vec3 _b3, vec3 _b4){

	origin = (_b1 + _b2 + _b3 + _b4)/4.0f;

	vec3 _xvec = (_b1 + _b2)/2.0f - origin;
	vec3 _yvec = (_b2 + _b3)/2.0f - origin;

	this->length = glm::length(_yvec);
	this->height = glm::length(_xvec);

	xvec = normalize(_xvec);
	yvec = normalize(_yvec);
	zvec = normalize(cross(xvec,yvec));

	if (zvec.z < 0){
		zvec *= -1.0f;
	}
}


RayHit *Plane::intersect_ray(Ray& r) {

    float denom = dot(zvec,r.dir); 

    if (denom < -0.001) { 

        float t = (dot(zvec,origin) - dot(zvec,r.origin)) / denom;

        if (t < .001) return nullptr; 

        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - origin;

        if (abs(dot(fromOrg, xvec) / this->height) > 1.0 || abs(dot(fromOrg, yvec) / this->length) > 1.0){
        	delete hit;
        	return nullptr; 	
        }

        RayHit *planehit = new RayHit(hit, new vec3(zvec), t, &r, this);
        return planehit;
    } 
    return nullptr; 
} 