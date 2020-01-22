#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include "Ray.h"
#include "Tri.h"

#include <iostream>

using namespace glm;
using namespace std;



Tri::Tri(vec3 p0, vec3 p1, vec3 p2){
	v0 = p0;
	v1 = p1;
	v2 = p2;
	origin = (v0 + v1 + v2)/3.0f;
}

/*RayHit *Tri::intersect_ray(Ray r) {

    vec3 v0v1 = v1 - v0; 
    vec3 v0v2 = v2 - v0; 

    // no need to normalize
    vec3 zvec = -1.0f * cross(v0v1,v0v2);

    float denom = dot(zvec,r.dir); 

    if (denom < -0.001) { 
        //cout << r.origin.x << " " << r.origin.y << " " << r.origin.z << endl;

        float t = (dot(zvec,origin) - dot(zvec,r.origin))/denom;
        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - origin;

        float u = 0;// dot(fromOrg, xvec) / this->height;
        float v = 0;//dot(fromOrg, yvec) / this->length;

        if (length(fromOrg) > 1.5f){
            return nullptr;     
        }

        RayHit *planehit = new RayHit(new vec3(*hit),new vec2((1+u)/2,(1+v)/2),new vec3(zvec),t);
        return planehit;
    } 
    return nullptr; 
}*/



RayHit *Tri::intersect_ray(Ray r) {

	//Credits to scratchpixel...

	vec3 orig = r.origin;
	vec3 dir = r.dir;
 
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
    if (abs(NdotRayDirection) < 0.0001f) // almost 0 
        return nullptr; // they are parallel so they don't intersect ! 
 
    // compute d parameter using equation 2
    float d = dot(N,v0); 
 
    // compute t (equation 3)
    t = (dot(N,orig) + d) / NdotRayDirection; 
    // check if the triangle is in behind the ray
    if (t < 0) return nullptr; // the triangle is behind 
 
    // compute the intersection point using equation 1
    //cout << "Hit length " << t << endl;
    
    vec3 hit = orig + t * dir; 
 
    // Step 2: inside-outside test
    vec3 C; // vector perpendicular to triangle's plane 
 
    // edge 0
    vec3 edge0 = v1 - v0; 
    vec3 vp0 = hit - v0; 
    C = cross(edge0,vp0); 

    if (dot(N,C) < 0) return nullptr; // P is on the right side 
 

    // edge 1
    vec3 edge1 = v2 - v1; 
    vec3 vp1 = hit - v1; 
    C = cross(edge1,vp1); 

    if ((u = dot(N,C)) < 0)  return nullptr; // P is on the right side 
 

    // edge 2
    vec3 edge2 = v0 - v2; 
    vec3 vp2 = hit - v2; 
    C = cross(edge2,vp2); 

    if ((v = dot(N,C)) < 0) return nullptr; // P is on the right side; 
 
    u /= denom; 
    v /= denom; 
 
    return new RayHit(new vec3(hit), new vec2(u,v), new vec3(normalize(N)), t); // this ray hits the triangle 				
}