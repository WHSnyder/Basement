#ifndef rayi
#define rayi

#include <glm.hpp>
#include <ext.hpp>

using namespace glm;



class Obj;



class Contact {

	public:

		vec3 normal, point;
		float moveback;
		int type;

		Contact(vec3 n, vec3 p, float m){
			
			normal = n;
			point = p;
			moveback = m;
		}

		Contact(vec3 n, vec3 p){
			
			normal = n;
			point = p;
		}
};

class Light {

	public:

		vec3 location;
		vec3 direction;
		vec3 color;

	Light(vec3 l, vec3 d, vec3 c){
		location = l;
		direction = normalize(d);
		color = c;
	}
};

class Ray {

	public:

		vec3 dir;
		vec3 origin;

		Ray(vec3 o, vec3 d){
			dir = normalize(d);
			origin = o;
		}		
};

class RayHit {

	public:

		vec3 *entrance;
		vec3 *ent_normal;
		float ent_distance;

		vec3 *exit;
		vec3 *exit_normal;
		float exit_distance;

		Ray *ray;
		Obj *object_hit;

		RayHit(vec3& w_ent, vec3& n_ent, float d_ent, vec3& w_exit, vec3& n_exit, float d_exit, Ray *r, Obj *obj_hit){
			
			entrance = new vec3(w_ent);
			ent_normal = new vec3(n_ent);
			ent_distance = d_ent;

			exit = new vec3(w_exit);
			exit_normal = new vec3(n_exit);
			exit_distance = d_exit;

			ray = r;
			object_hit = obj_hit;
		}

		RayHit(vec3 *w, vec3 *n, float d, Ray *r, Obj *obj_hit){

			entrance = w;
			ent_normal = n;
			ent_distance = d;
			exit_distance = 700000;

			exit_normal = nullptr;
			exit = nullptr;

			ray = r;
			object_hit = obj_hit;
		}

		RayHit(vec3 *w_ent, vec3 *n_ent, float d_ent, vec3 *w_exit, vec3 *n_exit, float d_exit, Ray *r, Obj *obj_hit){

			entrance = w_ent;
			ent_normal = n_ent;
			ent_distance = d_ent;

			exit = w_exit;
			exit_normal = n_exit;
			exit_distance = d_exit;

			ray = r;
			object_hit = obj_hit;
		}

		~RayHit(){

			delete entrance;
			delete ent_normal;
			delete exit;
			delete exit_normal;
		}
};
#endif