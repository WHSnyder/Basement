#include "Obj.h"
#include <iostream>

using namespace glm;
using namespace std;


/*
*  Plane methods.
*/

cv::Vec3b Plane::shade(RayHit *rhit, cv::Mat *img, Scene *scene){

	vec3 hit_pos = *rhit -> entrance;
    vec3 fromOrg = hit_pos - origin;
    std::vector<Light *> lights = scene -> lights;

	int i = -1;
	int row = (int) (img->rows - 1) * ((dot(fromOrg, xvec) / this->height) + 1.0f)/2.0f;
	int col = (int) (img->cols - 1) * ((dot(fromOrg, yvec) / this->length) + 1.0f)/2.0f;

	Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
	RayHit *shadow_hit = scene -> intersect_scene(shadow, &i);

	float dotprod = -1.0f * dot(*rhit -> ent_normal, lights[0]->direction);

	if (shadow_hit != nullptr || dotprod < .2f) dotprod = .2f;

	delete shadow_hit;

	return dotprod * img->at<cv::Vec3b>(row, col);
}


Plane::Plane(vec3 _b1, vec3 _b2, vec3 _b3, vec3 _b4){

	origin = (_b1 + _b2 + _b3 + _b4)/4.0f;

	vec3 _xvec = (_b1 + _b2)/2.0f - origin;
	vec3 _yvec = (_b2 + _b3)/2.0f - origin;

	this->length = glm::length(_yvec);
	this->height = glm::length(_xvec);

	xvec = normalize(_xvec);
	yvec = normalize(_yvec);
	zvec = normalize(cross(xvec,yvec));

	if (zvec.z < 0){
		zvec *= -1.0f;
	}
}


RayHit *Plane::intersect_ray(Ray& r) {

    float denom = dot(zvec,r.dir); 

    if (denom < -0.001) { 

        float t = (dot(zvec,origin) - dot(zvec,r.origin)) / denom;

        if (t < .001) return nullptr; 

        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - origin;

        if (abs(dot(fromOrg, xvec) / this->height) > 1.0 || abs(dot(fromOrg, yvec) / this->length) > 1.0){
        	delete hit;
        	return nullptr; 	
        }

        RayHit *planehit = new RayHit(hit, new vec3(zvec), t, &r, this);
        return planehit;
    } 
    return nullptr; 
} 


/*
*  Sphere methods.
*/


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

	return new RayHit(ent_hit, ent_normal, ent_length, exit_hit, exit_normal, exit_length, &r, this);
}

/*
*  Triangle methods.
*/

cv::Vec3b Tri::shade(RayHit *rhit, cv::Mat *tex, Scene *scene){

    cv::Vec3b col = cv::Vec3b(50,50,50);
    
    int i = -1;

    Ray reflection = Ray(*rhit->entrance,reflect(*rhit->ent_normal, rhit->ray->dir));
    RayHit *reflect_hit = scene -> intersect_scene(reflection, &i);

    if (reflect_hit == nullptr) return col;

    col = reflect_hit -> object_hit -> shade(reflect_hit, tex, scene);

    delete reflect_hit;

    return col;
}
 
RayHit *Tri::intersect_ray(Ray& r) {

    vec3 v0 = p0->coord, v1 = p1->coord, v2 = p2->coord;
    vec3 v0v1 = v1 - v0, v0v2 = v2 - v0; 
    vec3 zvec = normalize(cross(v0v1,v0v2));

    float denom = dot(zvec,r.dir); 

    if (denom > 0.001) { 

        float t = (dot(zvec,v0) - dot(zvec,r.origin)) / denom;

        if (t < 0) return nullptr;

        vec3 *hit = new vec3(r.origin + t * r.dir); 
        vec3 fromOrg = *hit - v0;

        float d00 = dot(v0v1, v0v1);
        float d01 = dot(v0v1, v0v2);
        float d11 = dot(v0v2, v0v2);
        float d20 = dot(fromOrg, v0v1);
        float d21 = dot(fromOrg, v0v2);
        denom = d00 * d11 - d01 * d01;

        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        if (u > 1.0 || v > 1.0 || w > 1.0 || u < 0.0 || v < 0.0 || w < 0.0){
            delete hit;
            return nullptr;
        }

        return new RayHit(hit, new vec3(zvec), t, &r, this);
    } 
    return nullptr; 
}