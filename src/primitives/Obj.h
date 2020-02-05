#ifndef glmi
#define glmi
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;
#endif 

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

#include "Ray.h"

#ifndef obji
#define obji

class Scene;


/*
* General shading functions.
*/


cv::Vec3b shade_reflective(RayHit *rhit, cv::Mat *tex, Scene *scene, int bounce);


struct Vertex {

	vec3 coord;
	vec3 normal;
	vec2 uv;
};


class Obj {

	public:
		vec3 origin;
		virtual RayHit *intersect_ray(Ray& r)=0;
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Scene *scene, int bounce)=0;
		cv::Vec3b (*shader)(RayHit *rh, cv::Mat *image, Scene *scene, int bounce) = nullptr;
};


class Plane : public Obj {

	public:
		vec3 xvec,yvec,zvec;
		float length,height;
		Plane(vec3 b1, vec3 b2, vec3 b3, vec3 b4);
		virtual RayHit *intersect_ray(Ray& r);
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Scene *scene, int bounce);
};


class Sphere : public Obj {

	public:
		float radius;
		vec3 color;
		Sphere(vec3 center, vec3 color, float r);
		virtual RayHit *intersect_ray(Ray& r);
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Scene *scene, int bounce);
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
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Scene *scene, int bounce);
};


class Cube : public Obj {

	public:
		vec3 min,max;
		Cube(vec3 _lb, vec3 _ub);
		virtual RayHit *intersect_ray(Ray& r);
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Scene *scene, int bounce);
};
#endif