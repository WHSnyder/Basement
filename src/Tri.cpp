#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include "Ray.h"
#include "Tri.h"

using namespace glm


		
Tri::Tri(vec3 _p0, vec3 _p1, vec3 _p2){
	v0 = _p0;
	v1 = _p1;
	v2 = _p2;
	origin = (v0 + v1 + v2)/3;
}

RayHit *Tri::intersect_ray(Ray r) {

	//Credits to scratchpixel...

	orig = r.origin;
	dir = r.dir;
 
    float t,u,v;

    // compute plane's normal
    vec3 v0v1 = v1 - v0; 
    vec3 v0v2 = v2 - v0; 

    // no need to normalize
    vec3 N = cross(v0v1,v0v2); // N 
    float denom = dot(N,N);
 
    // Step 1: finding P
 
    // check if ray and plane are parallel ?
    float NdotRayDirection = dot(N,dir); 
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
        return nullptr; // they are parallel so they don't intersect ! 
 
    // compute d parameter using equation 2
    float d = dot(N,v0); 
 
    // compute t (equation 3)
    t = (dot(N,orig) + d) / NdotRayDirection; 
    // check if the triangle is in behind the ray
    if (t < 0) return nullptr; // the triangle is behind 
 
    // compute the intersection point using equation 1
    vec3 hit = orig + t * dir; 
 
    // Step 2: inside-outside test
    vec3 C; // vector perpendicular to triangle's plane 
 
    // edge 0
    vec3 edge0 = v1 - v0; 
    vec3 vp0 = P - v0; 
    C = cross(edge0,vp0); 

    if (dot(N,C) < 0) return nullptr; // P is on the right side 
 
 
    // edge 1
    vec3 edge1 = v2 - v1; 
    vec3 vp1 = P - v1; 
    C = cross(edge1,vp1); 

    if ((u = dot(N,C)) < 0)  return nullptr; // P is on the right side 
 

    // edge 2
    vec3 edge2 = v0 - v2; 
    vec3 vp2 = P - v2; 
    C = cross(edge2,vp2); 

    if ((v = dot(N,C)) < 0) return nullptr; // P is on the right side; 
 
    u /= denom; 
    v /= denom; 
 
    return new RayHit(new vec3(hit), new vec2(u,v), new vec3(normalize(N)), t); // this ray hits the triangle 				
}



