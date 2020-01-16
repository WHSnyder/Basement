#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 


#include "Ray.h"

using namespace glm;

Ray::Ray(vec3 o, vec3 d){
	dir = normalize(d);
	origin = o;
}