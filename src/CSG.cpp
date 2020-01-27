#include "Ray.h"

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>

#include "CSG.h"


RayHit *CSG::intersect_ray(Ray &r){

	RayHit *other_hit, *this_hit;

	switch (op){

		case leaf: return shape -> intersect_ray(r);
				   break;

		case sub:  {	*this_hit = shape -> intersect_ray(r);  

						if (this_hit == nullptr){
							return nullptr;
						}

						other_hit = link -> intersect_ray(r);

						if (other_hit == nullptr){
							return this_hit;
						}
				   }

				   break;

		case un:   {	RayHit *other_hit

				   }


	}


	if (op == leaf){
		return shape -> intersect_ray(r);
	}
	else if (op == sub){

	}


}





