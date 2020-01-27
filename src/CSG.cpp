#include "Ray.h"

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>

#include "CSG.h"


RayHit *CSG::intersect_ray(Ray &r, bool mode){

	RayHit *other_hit, *this_hit;

	switch (op){

		case leaf: return shape -> intersect_ray(r);
				   break;

		case sub:  {	this_hit = shape -> intersect_ray(r);  

						if (this_hit == nullptr){
							return nullptr;
						}

						other_hit = link -> intersect_ray(r);

						if (other_hit == nullptr){
							return this_hit;
						}

						if (this_hit ->)




				   }
				   break;

		case un:   {	this_hit = shape -> intersect_ray(r);

						if (this_hit == nullptr){
							return link -> intersect_ray(r);
						}

						other_hit = link -> intersect_ray(r);

						if (other_hit == nullptr){
							return this_hit;
						}

						if (other_hit -> distance < this_hit -> distance){
							delete this_hit;
							return other_hit;
						}

						delete other_hit;
						return this_hit;
				   }
				   break;

	    case intx: {	this_hit = shape -> intersect_ray(r);

	    				if (this_hit == nullptr){
	    					return nullptr;
	    				}

	    				other_hit = link -> intersect_ray(r);

	    				if (other_hit == nullptr){
	    					delete this_hit;
	    					return nullptr;
	    				}

	    				if (other_hit -> distance < this_hit -> distance){
	    					delete other_hit;
	    					return this_hit;
	    				}

	    				delete this_hit;
	    				return other_hit;
	    		   }
	    		   break;
	}
	return nullptr;
}





