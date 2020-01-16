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


vec3 Sphere::intersect_ray(Ray r) {

	vec3 to_center = origin - r.origin;

	float dotprod = dot(to_center, r.dir);
	float cos = dotprod / length(to_center);

	vec3 center_to_line = origin - dotprod * r.dir + r.origin;
	float d = length(center_to_line);

	if (d >= radius){
		return vec3(0.0,0.0,0.0);
	}

	float hit_length = dotprod - sqrt( pow(radius,2) - pow(d,2) );
	hit_length = hit_length < 0 ? -1*hit_length : hit_length;

	return normalize( r.origin + hit_length * r.dir - origin);
}