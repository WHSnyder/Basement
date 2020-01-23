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


RayHit *Tri::intersect_ray(Ray r) {

    vec3 v0v1 = v1 - v0; 
    vec3 v0v2 = v2 - v0; 

    vec3 zvec = normalize(-1.0f*cross(v0v1,v0v2));

    float denom = dot(zvec,r.dir); 

    if (denom < -0.001) { 

        float t = (dot(zvec,v0) - dot(zvec,r.origin))/denom;

        if (t < 0) return nullptr;

        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - v0;

        float d00 = dot(v0v1, v0v1);
        float d01 = dot(v0v1, v0v2);
        float d11 = dot(v0v2, v0v2);
        float d20 = dot(fromOrg, v0v1);
        float d21 = dot(fromOrg, v0v2);
        denom = d00 * d11 - d01 * d01;

        float w = (d11 * d20 - d01 * d21) / denom;
        float v = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        if (abs(u) > 1.0 || abs(v) > 1.0 || abs(w > 1.0)){
            return nullptr;
        }

        return new RayHit(hit,new vec2((1+u)/2,(1+v)/2),new vec3(zvec),t);
    } 
    return nullptr; 
}