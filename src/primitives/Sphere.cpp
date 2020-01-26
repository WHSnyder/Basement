#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

using namespace glm;

#include "Sphere.h"

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

cv::Vec3b Sphere::shade(RayHit *rhit, cv::Mat *img, Obj *objects[], Light *lights[]){

	vec3 hit_pos = *rhit -> worldCoord;
	vec3 col = lights[0] -> color;

	int i = -1;

	float dotprod = -1.0f*dot(*rhit -> normal,lights[0]->direction);

	if (dotprod < 0.2){
		dotprod = .2;
	}
	else {

		Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
		RayHit *shadow_hit = intersect_scene(objects, shadow, &i);

		if (shadow_hit != nullptr) dotprod = .2;
		delete shadow_hit;
	}

	//return dotprod * cv::Vec3b(col.x,col.y,col.z).mul( cv::Vec3b(200,100,200) )/255;
	return dotprod * cv::Vec3b(color.x,color.y,color.z);  
}

Sphere::Sphere(vec3 center, vec3 diffuse, float r){
	origin = center;
	color = diffuse;
	radius = r;
}

RayHit *Sphere::intersect_ray(Ray& r) {

	vec3 to_center = origin - r.origin;

	float dotprod = dot(to_center, r.dir);

	vec3 center_to_line = origin - (dotprod * r.dir + r.origin);
	
	float d = length(center_to_line);
	if (d >= radius){	
		return nullptr;
	}

	float hit_length = dotprod - abs(sqrt(pow(radius,2) - pow(d,2)));

	if (hit_length < .001) return nullptr;

	vec3 *hit = new vec3( r.origin + hit_length * r.dir );

	return new RayHit(hit, new vec2(0,0), new vec3(normalize(*hit - origin)), hit_length, &r);
}