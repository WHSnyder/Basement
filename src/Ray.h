#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 


#ifndef ray
#define ray
using namespace glm;


class Ray {

	public:
		vec3 dir;
		vec3 origin;
		Ray(vec3 o, vec3 d);
};

#endif