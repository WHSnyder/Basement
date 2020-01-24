#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

using namespace glm;

#include "Ray.h"
#include "Sphere.h"

Sphere::Sphere(float x, float y, float z, float r){
	origin = vec3(x,y,z);
	radius = r;
}

RayHit *Sphere::intersect_ray(Ray r) {

	vec3 to_center = origin - r.origin;

	float dotprod = dot(to_center, r.dir);

	vec3 center_to_line = origin - (dotprod * r.dir + r.origin);
	
	float d = length(center_to_line);
	if (d >= radius){	
		return nullptr;
	}

	float hit_length = dotprod - abs(sqrt(pow(radius,2) - pow(d,2)));
	vec3 *hit = new vec3( r.origin + hit_length * r.dir );

	return new RayHit(hit, new vec2(0,0), new vec3(normalize(*hit - origin)), hit_length);
}