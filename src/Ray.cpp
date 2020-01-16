#ifndef glm
#define glm
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
using namespace glm;
#endif 


#include "Ray.h"


Ray::Ray(vec3 o, vec3 d){
	dir = normalize(d);
	origin = o;
}