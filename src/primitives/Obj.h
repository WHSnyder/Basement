#ifndef glmi
#define glmi
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;
#endif 

#include "Ray.h"
#include <vector>

#ifndef obji
#define obji

class Scene;


/*
* General shading functions.
*/


vec3 shade_reflective(RayHit *rhit, Scene *scene, int bounce);


//Tri distance function from http://www.iquilezles.org/www/articles/triangledistance/triangledistance.htm

float udTriangle(vec3 v1, vec3 v2, vec3 v3, vec3 p){

    // prepare data    
    vec3 v21 = v2 - v1; vec3 p1 = p - v1;
    vec3 v32 = v3 - v2; vec3 p2 = p - v2;
    vec3 v13 = v1 - v3; vec3 p3 = p - v3;
    vec3 nor = cross( v21, v13 );

    return sqrt( // inside/outside test    
                 (sign(dot(cross(v21,nor),p1)) + 
                  sign(dot(cross(v32,nor),p2)) + 
                  sign(dot(cross(v13,nor),p3))<2.0) 
                  ?
                  // 3 edges    
                  min( min( 
                  dot2(v21*clamp(dot(v21,p1)/dot2(v21),0.0,1.0)-p1), 
                  dot2(v32*clamp(dot(v32,p2)/dot2(v32),0.0,1.0)-p2) ), 
                  dot2(v13*clamp(dot(v13,p3)/dot2(v13),0.0,1.0)-p3) )
                  :
                  // 1 face    
                  dot(nor,p1)*dot(nor,p1)/dot2(nor) );
}


/*


int sphere_shrunk_gjk(Sphere *a, Sphere *b, vec3 *normal, vec3 *pt){

	Simplex simp = Simplex();

	vec3 init = vec3(0,-1,0);
	vec3 s1 = a -> support(init), s2 = b -> support(-init); 

	vec3 support_pt = s1 - s2, next_pt;
	
	vec3 next_dir = vec3(0,0,0) - support_pt;

	s1 = a -> support(next_dir);
	s2 = b -> support(-next_dir);

	next_pt = s1 - s2;

	simp.verts.push_back(support_pt);
	simp.verts.push_back(next_pt);

	vec3 diff = normalize( next_pt - support_pt );

	float proj = dot(diff, -next_pt);

	diff *= proj;

	s1 = a -> support(diff);
	s2 = b -> support(-diff);

	simp.verts.push_back(s2 - s1);















}





struct Simplex {
	std::vector<vec3> verts;
};


struct TriPrim {
	int a,b,c;
};
*/

struct Vertex {

	vec3 coord;
	vec3 normal;
	vec2 uv;
};


struct Material {

	int rows,cols;
	unsigned char *data;
};


class Sphere;


class Obj {

	public:

		vec3 origin, force, vel = vec3(0,0,0);
		Material *mat;
		int is_static;

		virtual RayHit *intersect_ray(Ray& r)=0;
		virtual vec3 shade(RayHit *rh, Scene *scene, int bounce)=0;
		virtual Contact *collide_sphere(Sphere *s, int mode)=0;
		//virtual vec3 support(vec3 dir)=0;

		vec3 (*shader)(RayHit *rh, int bounce) = nullptr;
};


class Sphere : public Obj {

	public:
		float radius;
		vec3 color;

		Sphere(vec3 center, vec3 color, float r);

		virtual RayHit *intersect_ray(Ray& r);
		//virtual vec3 support(vec3 dir);
		virtual vec3 shade(RayHit *rh, Scene *scene, int bounce);
		virtual Contact *collide_sphere(Sphere *s, int mode);
};


class Plane : public Obj {

	public:
		vec3 xvec,yvec,zvec;
		float length,height;

		Plane(vec3 b1, vec3 b2, vec3 b3, vec3 b4, Material *mat);

		virtual RayHit *intersect_ray(Ray& r);
		//virtual vec3 support(vec3 dir);
		virtual vec3 shade(RayHit *rh, Scene *scene, int bounce);
		virtual Contact *collide_sphere(Sphere *s, int mode);
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

		virtual RayHit *intersect_ray(Ray& r);
		//virtual vec3 support(vec3 dir)=0;
		virtual vec3 shade(RayHit *rh, Scene *scene, int bounce);
		virtual Contact *collide_sphere(Sphere *s, int mode);
};


class Cyl : public Obj {

	public:

		float radius,height;
		vec3 axis,origin;

		Cyl(float rad, float h, vec3 o, vec3 ax){

			axis = ax;
			origin = o;
			radius = rad;
			height = h;
		}

		virtual RayHit *intersect_ray(Ray& r);
		//virtual vec3 support(vec3 dir);
		virtual vec3 shade(RayHit *rh, Scene *scene, int bounce);
		virtual Contact *collide_sphere(Sphere *s, int mode);
};


class Cube : public Obj {

	public:
		vec3 min,max, up, right, forward;
		float updim,rightdim,forwarddim;

		Cube(vec3 _lb, vec3 _ub);

		virtual RayHit *intersect_ray(Ray& r);
		//virtual vec3 support(vec3 dir);
		virtual vec3 shade(RayHit *rh, Scene *scene, int bounce);
		virtual Contact *collide_sphere(Sphere *s, int mode);
};
#endif