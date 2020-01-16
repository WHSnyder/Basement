#ifndef glm
#define glm
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
using namespace glm;
#endif 


#ifndef ray
#define ray


class Ray {

	public:
		vec3 dir;
		vec3 origin;
		Ray(vec3 o, vec3 d);
};

#endif