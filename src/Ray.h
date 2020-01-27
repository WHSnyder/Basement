#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#ifndef rayi
#define rayi
using namespace glm;

struct Surface {
	vec3 location;
	vec3 normal;
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

		RayHit(vec3 *w, vec3 *n, float d, Ray *r){
			entrance = w;
			ent_normal = n;
			ent_distance = d;
			exit_distance = 700000;

			exit_normal = nullptr;
			exit = nullptr;

			ray = r;
		}

		RayHit(vec3 *w_ent, vec3 *n_ent, float d_ent, vec3 *w_exit, vec3 *n_exit, float d_exit, Ray *r){
			entrance = w_ent;
			ent_normal = n_ent;
			ent_distance = d_ent;

			exit = w_exit;
			exit_normal = n_exit;
			exit_distance = d_exit;

			ray = r;
		}

		/*RayHit(RayHit *ex, RayHit *en, Ray *r){
			entrance = w_ent;
			ent_normal = n_ent;
			ent_distance = d_ent;

			exit = w_exit;
			exit_normal = n_exit;
			exit_distance = exit_distance;

			ray = r;
		}*/

		~RayHit(){
			delete entrance;
			delete ent_normal;
			delete exit;
			delete exit_normal;
		}
};
#endif