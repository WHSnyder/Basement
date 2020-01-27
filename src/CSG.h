#ifndef csgi
#define csgi


#include "Ray.h"
#include "Obj.h"

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>


enum optype {un,sub,intx,leaf};

class CSG {

	public:

		Obj *shape;
		CSG *link;
		optype op;


		CSG(Obj *object){
			shape = object;
			op = leaf;
		}

		CSG(CSG *operand, optype _op){

			if (_op == leaf) _op = un;
			link = operand;
			op = _op;
			shape = null;
		}


		virtual RayHit *intersect_ray(Ray& r);


		CSG * operator || (CSG *other){
			return new CSG(other, un);
		}

		CSG * operator && (CSG *other){
			return new CSG(other, intx);
		}

		CSG * operator - (CSG *other){
			return new CSG(other, sub);
		}

	
};

#endif
