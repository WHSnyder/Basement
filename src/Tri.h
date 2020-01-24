#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include "Ray.h"
#include "Obj.h"

using namespace glm;

#ifndef tria
#define tria

struct Vertex {

	vec3 coord;
	vec3 normal;
	vec2 uv;
};

class Tri : public Obj {

	public :
		Vertex *p0,*p1,*p2;

		Tri(Vertex *_p0, Vertex *_p1, Vertex *_p2){

			p0 = _p0;
			p1 = _p1;
			p2 = _p2;
			origin = (_p0->coord + _p1->coord + _p2->coord)/3.0f;
		}

	    Tri(vec3 _p0, vec3 _p1, vec3 _p2){

	    	p0 = new Vertex(), p1 = new Vertex(), p2 = new Vertex();

			p0 -> coord = _p0;
			p1 -> coord = _p1;
			p2 -> coord = _p2;
			origin = (_p0 + _p1 + _p2)/3.0f;
		}

		virtual RayHit *intersect_ray(Ray r);
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat image, Obj *objects[], Light *lights[]);
};
#endif