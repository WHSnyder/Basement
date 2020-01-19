#include <glm/glm.hpp> 
#include <glm/ext.hpp>

#include "Ray.h"
#include "Obj.h"

using namespace glm

#ifndef tria
#define tria


class Tri {

	public :
		vec3 v1,v2,v3;
		
		Tri(vec3 _p1, vec3 _p2, vec3 _p3){
			v1 = _p1;
			v2 = _p2;
			v3 = _p3;
			origin = (v1 + v2 + v3)/3;
		}

		vec3 intersect(Ray r){

			const Vec3f &orig, const Vec3f &dir, 
    const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, 
    float &t, float &u, float &v) 
{ 
    // compute plane's normal
    Vec3f v0v1 = v1 - v0; 
    Vec3f v0v2 = v2 - v0; 
    // no need to normalize
    Vec3f N = v0v1.crossProduct(v0v2); // N 
    float denom = N.dotProduct(N); 
 
    // Step 1: finding P
 
    // check if ray and plane are parallel ?
    float NdotRayDirection = N.dotProduct(dir); 
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
        return false; // they are parallel so they don't intersect ! 
 
    // compute d parameter using equation 2
    float d = N.dotProduct(v0); 
 
    // compute t (equation 3)
    t = (N.dotProduct(orig) + d) / NdotRayDirection; 
    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind 
 
    // compute the intersection point using equation 1
    Vec3f P = orig + t * dir; 
 
    // Step 2: inside-outside test
    Vec3f C; // vector perpendicular to triangle's plane 
 
    // edge 0
    Vec3f edge0 = v1 - v0; 
    Vec3f vp0 = P - v0; 
    C = edge0.crossProduct(vp0); 
    if (N.dotProduct(C) < 0) return false; // P is on the right side 
 
    // edge 1
    Vec3f edge1 = v2 - v1; 
    Vec3f vp1 = P - v1; 
    C = edge1.crossProduct(vp1); 
    if ((u = N.dotProduct(C)) < 0)  return false; // P is on the right side 
 
    // edge 2
    Vec3f edge2 = v0 - v2; 
    Vec3f vp2 = P - v2; 
    C = edge2.crossProduct(vp2); 
    if ((v = N.dotProduct(C)) < 0) return false; // P is on the right side; 
 
    u /= denom; 
    v /= denom; 
 
    return true; // this ray hits the triangle 


				
		}
}

#endif

