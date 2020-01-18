#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

using namespace glm;

#include "Ray.h"
#include "Plane.h"


Plane::Plane(vec3 _b1, vec3 _b2, vec3 _b3, vec3 _b4){

	origin = (_b1 + _b2 + _b3 + _b4)/4;

	_xvec = (_b1 + _b2)/2 - origin;
	_yvec = (_b2 + _b3)/2 - origin;

	length = length(_xvec);
	height = length(_yvec);

	xvec = normalize(_xvec);
	yvec = normalize(_yvec);
	zvec = cross(xvec,yvec);

	if (zvec.y < 0){
		zvec *= -1;
	}
}


vec3 Plane::intersect_ray(Ray r) {

    float denom = dotProduct(zvec, r.dir); 

    if (denom > 1e-6) { 
        vec3 to_plane = origin - r.origin; 
        t = dotProduct(to_plane, zvec) / denom; 

        vec3 hit = r.origin + t * r.dir; 
        vec3 fromOrg = hit - origin;

        u = dot(fromOrg, xvec) / length;
        v = dot(fromOrg, yvec) / height;

        if (abs(u) > 1 || abs(v) > 1){
        	return vec3(0.0,0.0,0.0); 	
        }
        return vec3(u,v,1);
    } 
 
    return vec3(0.0,0.0,0.0); 
} 
