#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

using namespace glm;

#include "Ray.h"
#include "Plane.h"

#include <iostream>
using namespace std;

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


RayHit *Plane::intersect_ray(Ray r) {

    float denom = dot(zvec,r.dir); 

    if (denom < -0.001) { 
    	//cout << r.origin.x << " " << r.origin.y << " " << r.origin.z << endl;

        float t = (dot(zvec,origin) - dot(zvec,r.origin))/denom;
        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - origin;

        float u = dot(fromOrg, xvec) / this->height;
        float v = dot(fromOrg, yvec) / this->length;

        if (abs(u) > 1.0 || abs(v) > 1.0){
        	return nullptr; 	
        }

        RayHit *planehit = new RayHit(hit,new vec2((1+u)/2,(1+v)/2),new vec3(zvec),t);
        return planehit;
    } 
    return nullptr; 
} 
