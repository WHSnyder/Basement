#include "Sphere.h"

using namespace glm;


cv::Vec3b Sphere::shade(RayHit *rhit, cv::Mat *img, Scene *scene){

	vec3 hit_pos = *rhit -> entrance;
	std::vector<Light *> lights = scene -> lights;
	vec3 col = lights[0] -> color;

	int i = -1;

	float dotprod = -1.0f*dot(*rhit -> ent_normal,lights[0]->direction);

	if (dotprod < 0.2){
		dotprod = .2;
	}
	else {

		Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
		RayHit *shadow_hit = scene -> intersect_scene(shadow, &i);

		if (shadow_hit != nullptr) dotprod = .2;
		delete shadow_hit;
	}

	return dotprod * cv::Vec3b(color.x,color.y,color.z);  
}

Sphere::Sphere(vec3 center, vec3 diffuse, float r){
	origin = center;
	color = diffuse;
	radius = r;
}

RayHit *Sphere::intersect_ray(Ray& r) {

	vec3 to_center = origin - r.origin;

	float dotprod = dot(to_center, r.dir);

	vec3 center_to_line = origin - (dotprod * r.dir + r.origin);
	
	float d = length(center_to_line);
	if (d >= radius){	
		return nullptr;
	}

	float temp = abs(sqrt(radius*radius - d*d));
	float ent_length = dotprod - temp;
	float exit_length = dotprod + temp;

	if (ent_length < .001) return nullptr;

	vec3 *ent_hit = new vec3( r.origin + ent_length * r.dir );
	vec3 *exit_hit = new vec3( r.origin + exit_length * r.dir );

	vec3 *ent_normal = new vec3( normalize(*ent_hit - origin) );
	vec3 *exit_normal = new vec3( normalize(*exit_hit - origin) );

	return new RayHit(ent_hit, ent_normal, ent_length, exit_hit, exit_normal, exit_length, &r);
}