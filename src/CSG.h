#ifndef csgi
#define csgi

#include "primitives/Obj.h"


enum optype {un,sub,intx,leaf};

class CSG {

	public:

		Obj *shape;
		CSG *link;
		optype op;

		CSG(Obj *object){
			
			shape = object;
			op = leaf;
			link = nullptr;
		}

		CSG(CSG *orig, CSG *operand, optype _op){

			if (_op == leaf) _op = un;

			link = operand;
			op = _op;
			shape = orig -> shape;
		}

		RayHit *intersect_ray(Ray& r);

		CSG * operator || (CSG& other){
			return new CSG(this, &other, un);
		}

		CSG * operator && (CSG& other){
			return new CSG(this, &other, intx);
		}

		CSG * operator - (CSG& other){
			return new CSG(this, &other, sub);
		}
};
#endif