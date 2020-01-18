#include <glm/glm.hpp> 
#include <glm/ext.hpp>

#include "Ray.h"
#include "Obj.h"

using namespace glm


class Tri : public Obj {

	public :
		vec3 p1,p2,p3;
		
		Tri(vec3 _p1, vec3 _p2, vec3 _p3){
			p1 = _p1;
			p2 = _p2;
			p3 = _p3;
			origin = (p1 + p2 + p3)/3;
		}

		vec3 intersect(Ray r){

				
		}
}


