#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

using namespace glm;
using namespace std;

#include <iostream>

#include "Ray.h"
#include "Plane.h"


void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}


Plane::Plane(vec3 _b1, vec3 _b2, vec3 _b3, vec3 _b4){

	origin = (_b1 + _b2 + _b3 + _b4)/4.0f;

	vec3 _xvec = (_b1 + _b2)/2.0f - origin;
	vec3 _yvec = (_b2 + _b3)/2.0f - origin;

	this->length = glm::length(_xvec);
	this->height = glm::length(_yvec);

	xvec = normalize(_xvec);
	yvec = normalize(_yvec);
	zvec = cross(xvec,yvec);

	if (zvec.y < 0){
		zvec.y *= -1.0f;
	}

	cout << "============" << endl;
	printVec("xvec",xvec);
	printVec("yvec",yvec);
	printVec("zvec",zvec);
	printVec("origin",origin);
	cout << "Length: " << this->length << " Width: " << this->height << endl;
}


vec3 Plane::intersect_ray(Ray r) {

    float denom = -1.0 * dot(zvec, r.dir); 

    if (denom > 0.01) { 

        float t = dot(zvec,origin-r.origin)/denom;
        vec3 hit = r.origin + t * r.dir; 
        vec3 fromOrg = hit - origin;

        float u = dot(fromOrg, xvec) / this->length;
        float v = dot(fromOrg, yvec) / this->height;

        if (abs(u) > 1.0 || abs(v) > 1.0){
        	return vec3(0.0,0.0,0.0); 	
        }
        return vec3((1+u)/2,(1+v)/2,1.0);
    } 
    return vec3(0.0,0.0,0.0); 
} 
