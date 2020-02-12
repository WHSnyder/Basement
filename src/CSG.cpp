#include <math.h>
#include "CSG.h"
#include <iostream>

using namespace std;


Contact *CSG::collide_sphere(Sphere *sphere, int mode){

	Contact *this_coll,*other_coll;

	switch (op) {

		case leaf:  
					return shape -> collide_sphere(sphere,mode);
					
					break;

		case sub:  	
					this_coll = shape -> collide_sphere(sphere,mode);  

					if (this_coll == nullptr) return nullptr;

					other_coll = link -> collide_sphere(sphere,mode);

					if (other_coll == nullptr) return this_coll;

					

					/*if (other_coll != nullptr) {
						
						delete link -> cached;
						link -> cached = other_coll;
						delete this_coll;

						return nullptr;
					}

					if (other_coll == nullptr && link -> cached != nullptr){

						delete this_coll;
						other_coll = link -> cached;
						link -> cached = nullptr;
						other_coll -> normal *= -1.0f;

						return other_coll;
					}

					return this_coll;*/
				   
				    break;

		case un:   	
					this_coll = shape -> collide_sphere(sphere,mode);
					
					if (this_coll == nullptr) return link -> collide_sphere(sphere,mode);

					return this_coll;

					break;

	    case intx: 
	    			this_coll = shape -> collide_sphere(sphere,mode);
					
					if (this_coll == nullptr) return nullptr;
	    			
	    			other_coll = link -> collide_sphere(sphere,mode);

					if (other_coll == nullptr){
						delete this_coll;
						return nullptr;
					}

					delete this_coll;
					return other_coll;

	    		    break;
	}
	return nullptr;
}




RayHit *CSG::intersect_ray(Ray &r){

	RayHit *other_hit, *this_hit, *composite;

	switch (op){

		case leaf:  
					return shape -> intersect_ray(r);
				    break;

		case sub:  	
					this_hit = shape -> intersect_ray(r);  
					if (this_hit == nullptr) return nullptr;
					
					other_hit = link -> intersect_ray(r);
					if (other_hit == nullptr) return this_hit;

					if (this_hit -> ent_distance <= other_hit -> ent_distance){
						delete other_hit;
						return this_hit;
					}
					
					if (this_hit -> exit_distance >= other_hit -> exit_distance){

						if (this_hit -> ent_distance >= other_hit -> exit_distance){
							delete other_hit;
							return this_hit;
						}
						
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

					delete other_hit;
					delete this_hit;
					return nullptr;
				   
				    break;

		case un:   	
					this_hit = shape -> intersect_ray(r);
					if (this_hit == nullptr) return link -> intersect_ray(r);

					other_hit = link -> intersect_ray(r);
					if (other_hit == nullptr) return this_hit;
					
					if (other_hit -> ent_distance < this_hit -> ent_distance){
						delete this_hit;
						return other_hit;
					}

					delete other_hit;
					return this_hit;
				   
				    break;

	    case intx: 
	    			this_hit = shape -> intersect_ray(r);
					if (this_hit == nullptr) return nullptr;
	    			
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

	    		    break;
	}
	return nullptr;
}