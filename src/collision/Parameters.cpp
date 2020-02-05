#include "Obj.h"
#include "Scene.h"
#include <iostream>
#include <vector>

using namespace glm;
using namespace std;

/*
*  Methods used for generating details of collisions such as penetration distance, collision normal.  
*  Doesn't handle collision responses, that's up to intepretation by CSG functions.
*/


/*
*  Plane methods.
*/

RayHit *Plane::collide_sphere(Ray& r) {

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


/*
*  Sphere methods.
*/

RayHit *Sphere::collide_sphere(Ray& r) {

	vec3 to_center = origin - r.origin;

	float dotprod = dot(to_center, r.dir);

	vec3 center_to_line = origin - (dotprod * r.dir + r.origin);
	
	float d = length(center_to_line);
	
	if (d >= radius) return nullptr;
	
	float temp = abs(sqrt(radius*radius - d*d));
	float ent_length = dotprod - temp, exit_length = dotprod + temp;

	if (ent_length < .001) return nullptr;

	vec3 *ent_hit = new vec3( r.origin + ent_length * r.dir );
	vec3 *exit_hit = new vec3( r.origin + exit_length * r.dir );

	vec3 *ent_normal = new vec3( normalize(*ent_hit - origin) );
	vec3 *exit_normal = new vec3( normalize(*exit_hit - origin) );

	return new RayHit(ent_hit, ent_normal, ent_length, exit_hit, exit_normal, exit_length, &r, this);
}


/*
*  Triangle methods.
*/

 RayHit *Tri::collide_sphere(Ray& r) {

    vec3 v0 = p0->coord, v1 = p1->coord, v2 = p2->coord;
    vec3 v0v1 = v1 - v0, v0v2 = v2 - v0; 
    vec3 zvec = normalize(cross(v0v1,v0v2));

    float denom = dot(zvec,r.dir); 

    if (denom > 0.001) { 

        float t = (dot(zvec,v0) - dot(zvec,r.origin)) / denom;

        if (t < 0) return nullptr;

        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - v0;

        float d00 = dot(v0v1, v0v1);
        float d01 = dot(v0v1, v0v2);
        float d11 = dot(v0v2, v0v2);
        float d20 = dot(fromOrg, v0v1);
        float d21 = dot(fromOrg, v0v2);
        denom = d00 * d11 - d01 * d01;

        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        if (u > 1.0 || v > 1.0 || w > 1.0 || u < 0.0 || v < 0.0 || w < 0.0){
            delete hit;
            return nullptr;
        }

        return new RayHit(hit, new vec3(zvec), t, &r, this);
    } 
    return nullptr; 
}



RayHit *Cube::collide_sphere(Ray &r) {

    float tmin,tmax,tymin,tymax,tzmin,tzmax,temp,xflip=1.0f,yflip=1.0f,zflip=1.0f;
    vec3 ent,exit,ent_normal,exit_normal,min_norm,max_norm; 
    RayHit *hit;

    tmin = 100000000;
    tmax = -100000000;

    if (abs(r.dir.x) > 0.0001){

	    tmin = (min.x - r.origin.x) / r.dir.x; 
	    tmax = (max.x - r.origin.x) / r.dir.x;

	    if (tmin > tmax){
	    	temp = tmax;
	    	tmax = tmin;
	    	tmin = temp;

	    	xflip = -1.0f;
	    }

	    min_norm = xflip * xbase;
    	max_norm = xflip * -1.0f * xbase;
	}

    if (abs(r.dir.y) > .00001){
 
	    tymin = (min.y - r.origin.y) / r.dir.y; 
	    tymax = (max.y - r.origin.y) / r.dir.y;

	    if (tymin > tymax){
	    	temp = tymax;
	    	tymax = tymin;
	    	tymin = temp;

	    	yflip = -1.0f;
	    } 
	 
	    if ((tmin > tymax) || (tymin > tmax)){ 
	        return nullptr; 
	    }

	    if (tymin > tmin){ 
	        tmin = tymin;
	        min_norm = yflip * -1.0f * ybase;
	    }

	    if (tymax < tmax){ 
	        tmax = tymax; 
	        max_norm = yflip * ybase;
	    }
	}

	if (abs(r.dir.z) > .00001){
 
	    tzmin = (min.z - r.origin.z) / r.dir.z; 
	    tzmax = (max.z - r.origin.z) / r.dir.z;

	    if (tzmin > tzmax){
	    	temp = tzmax;
	    	tzmax = tzmin;
	    	tzmin = temp;

	    	zflip = -1.0f;
	    }  
	 
	    if ((tmin > tzmax) || (tzmin > tmax)){
	        return nullptr; 
	    }

	    if (tzmin > tmin){ 
	        tmin = tzmin; 
	        min_norm = zflip * -1.0f * zbase;
	    }

	    if (tzmax < tmax){
	        tmax = tzmax;
	        max_norm = yflip * ybase; 
	    }
	}

    if (tmin < 0){
    	return nullptr;
	}

    ent = r.origin + tmin * r.dir;
    exit = r.origin + tmax * r.dir;

    hit = new RayHit(ent,min_norm,tmin,exit,max_norm,tmax,&r,this);

    return hit; 
}