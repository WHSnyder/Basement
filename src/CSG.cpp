#include "Ray.h"

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>

#include "CSG.h"


RayHit *CSG::intersect_ray(Ray &r, bool mode){

	RayHit *other_hit, *this_hit, *composite;

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

						if (this_hit -> exit_distance > other_hit -> exit_distance){
							
							*other_hit -> exit_normal *= -1.0f;
							other_hit -> ent_normal = other_hit -> exit_normal;
							other_hit -> ent_distance = other_hit -> exit_distance;
							other_hit -> entrance = other_hit -> exit;

							other_hit -> exit = new vec3(*this_hit->exit);
							other_hit -> exit_normal = new vec3(*this_hit->exit_normal);
							other_hit -> exit_distance = this_hit->exit_distance;

							delete this_hit;
							return other_hit;
						}

						delete this_hit;
						delete other_hit;

						return nullptr;
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

						if (other_hit -> ent_distance < this_hit -> ent_distance){
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

	    				if (other_hit -> ent_distance < this_hit -> ent_distance){
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





