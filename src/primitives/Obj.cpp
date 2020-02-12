#include "Obj.h"
#include "Scene.h"
#include <iostream>
#include <vector>

using namespace glm;
using namespace std;



vec3 shade_reflective(RayHit *rhit, Scene *scene, int bounce){

	vec3 col = vec3(100,100,100);

	if (bounce == 0) return col;
    
    Ray reflection = Ray(*rhit->entrance,reflect(*rhit -> ent_normal, rhit -> ray->dir));
    RayHit *reflect_hit = scene -> intersect_scene(reflection);

    if (reflect_hit == nullptr) return col;

    col = reflect_hit -> object_hit -> shade(reflect_hit, scene, bounce - 1);

    delete reflect_hit;

    return col;
}


/*
*	Cylinder methods.
*/
/*
cv::Vec3b Cyl::shade(RayHit *rhit, Scene *scene, int bounce){

	if (bounce == 0) return cv::Vec3b(50,50,50);

	if (shader != nullptr) return 0.8f * shader(rhit,img,scene,bounce);

	vec3 hit_pos = *rhit -> entrance;
	std::vector<Light *> lights = scene -> lights;

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

	return dotprod * cv::Vec3b(color.x,color.x,color.z);  	
}


RayHit *Cyl::intersect_ray(Ray& r) {

	vec3 to_center = origin - r.origin;

	float dotprod = dot(to_center, r.dir);

	vec3 center_to_line = origin - (dotprod * r.dir + r.origin);
	
	float d = length(center_to_line);
	
	if (d >= radius) return nullptr;
	
	float temp = abs(sqrt(radius*radius - d*d));
	float ent_length = dotprod - temp, exit_length = dotprod + temp;

	if (ent_length < .001) return nullptr;

	vec3 *ent_hit = new vec3( r.origin + ent_length * r.dir );
	vec3 *exit_hit = new vec3( r.origin + exit_length * r.dir );

	vec3 *ent_normal = new vec3( normalize(*ent_hit - origin) );
	vec3 *exit_normal = new vec3( normalize(*exit_hit - origin) );

	return new RayHit(ent_hit, ent_normal, ent_length, exit_hit, exit_normal, exit_length, &r, this);
}

*/



/*
*  Plane methods.
*/

Contact *Plane::collide_sphere(Sphere *sphere, int mode){

	vec3 center_to_plane = origin - sphere -> origin, new_orig;
	float dist_to_plane = -1.0f * dot(zvec, center_to_plane);
	Contact *ct = nullptr;

	if (dist_to_plane < sphere -> radius){
		
		//cout << "Hit: " << dist_to_plane << endl; 
		//vel_mag = length(test_sphere -> vel);
		//move_back =  dot(test_sphere -> vel / vel_mag,-1.0 * test_plane -> zvec)
		//pen_dist =

		ct = new Contact(zvec, sphere -> origin + (sphere -> radius - dist_to_plane) * zvec);

		//test_sphere -> vel = .9f * reflect(test_plane -> zvec, test_sphere -> vel);
	}

	return ct;
}


vec3 Plane::shade(RayHit *rhit, Scene *scene, int bounce){

	vec3 result = vec3(50,50,50);

	if (bounce == 0) return result;

	if (shader != nullptr) return 0.8f * shader(rhit, bounce);

	vec3 hit_pos = *rhit -> entrance;
    vec3 fromOrg = hit_pos - origin;
    std::vector<Light *> lights = scene -> lights;

	int start = 0;
	int row = (int) (mat->rows - 1) * ((dot(fromOrg, xvec) / this->height) + 1.0f) / 2.0f;
	int col = (int) (mat->cols - 1) * ((dot(fromOrg, yvec) / this->length) + 1.0f) / 2.0f;

	Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
	RayHit *shadow_hit = scene -> intersect_scene(shadow);

	float dotprod = -1.0f * dot(*rhit -> ent_normal, lights[0]->direction);

	if (shadow_hit != nullptr || dotprod < .2f) dotprod = .2f;

	delete shadow_hit;

	start = 3 * row * mat -> cols + col;
	result[0] = mat -> data[start++];
	result[1] = mat -> data[start++];
	result[2] = mat -> data[start];

	return dotprod * result;
}


Plane::Plane(vec3 _b1, vec3 _b2, vec3 _b3, vec3 _b4, Material *_mat){

	mat = _mat;

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

	if (zvec.x < 0){
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


vec3 Sphere::support(vec3 dir){

	return origin + radius * normalize(dir);
}*/


Contact *Sphere::collide_sphere(Sphere *s0, int mode){

	vec3 to_center = origin - s0 -> origin, new_orig, normal;
	float dist,l,r, move_back;
	Contact *result = nullptr;

	dist = length(to_center);

	//Test for normal collision
	if (mode == 1){
		if (dist < radius + s0 -> radius){

			normal = -1.0f * normalize(to_center);
			new_orig = s0 -> origin + (s0 -> radius - (dist - radius)) * normal;

			result = new Contact(normal, new_orig);
		}
	}
	//Test for inverse collision
	else {
		if (dist >  radius - s0 -> radius && dot(to_center,s0 -> vel) < 0){



			normal = normalize(to_center);
			new_orig = s0 -> origin + (dist - radius) * normal;

			result = new Contact(normal, new_orig);
		}
	}

	return result;
}


vec3 Sphere::shade(RayHit *rhit, Scene *scene, int bounce){

	if (bounce == 0) return vec3(50,50,50);

	if (shader != nullptr) return 0.8f * shader(rhit, bounce);

	vec3 hit_pos = *rhit -> entrance;
	std::vector<Light *> lights = scene -> lights;

	int i = -1;

	float dotprod = -1.0f*dot(*rhit -> ent_normal,lights[0]->direction);

	if (dotprod < 0.2){
		
		dotprod = .2;
	}
	else {

		Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
		RayHit *shadow_hit = scene -> intersect_scene(shadow);

		if (shadow_hit != nullptr) dotprod = .2;
		delete shadow_hit;
	}

	return dotprod * color;  	
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
	
	if (d >= radius) return nullptr;
	
	float temp = abs(sqrt(radius*radius - d*d));
	float ent_length = dotprod - temp, exit_length = dotprod + temp;

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


Contact *Tri::collide_sphere(Sphere *sphere, int mode){
	return nullptr;
}


vec3 Tri::shade(RayHit *rhit, Scene *scene, int bounce){

	vec3 col = vec3(50,50,50);

	if (bounce == 0) return col;

    Ray reflection = Ray(*rhit->entrance,reflect(*rhit->ent_normal, rhit->ray->dir));
    RayHit *reflect_hit = scene -> intersect_scene(reflection);

    if (reflect_hit == nullptr) return col;

    col = reflect_hit -> object_hit -> shade(reflect_hit, scene, bounce - 1);

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


/*
* Cube methods.
*/

vec3 xbase = vec3(1,0,0);
vec3 ybase = vec3(0,1,0);
vec3 zbase = vec3(0,0,1);




Contact *Cube::collide_sphere(Sphere *sphere, int mode){
	return nullptr;
}


vec3 Cube::shade(RayHit *rhit, Scene *scene, int bounce){

	if (bounce == 0) return vec3(50,50,50);

	vec3 hit_pos = *rhit -> entrance, color = vec3(250,20,230);
	std::vector<Light *> lights = scene -> lights;

	int i = -1;

	float dotprod = -1.0f*dot(*rhit -> ent_normal,lights[0]->direction);

	if (dotprod < 0.2){
		dotprod = .2;
	}
	else {

		Ray shadow = Ray(hit_pos, lights[0]->location - hit_pos);
		RayHit *shadow_hit = scene -> intersect_scene(shadow);

		if (shadow_hit != nullptr) dotprod = .2;
		delete shadow_hit;
	}

	return dotprod * color;
}


Cube::Cube(vec3 lb, vec3 ub){

	origin = (lb + ub)/2.0f;
	min = lb;
	max = ub;

	up = (max - origin) * vec3(0,0,1);
	updim = length(up);

	forward = (max - origin) * vec3(0,1,0);
	forwarddim = length(forward);

	right = (max - origin) * vec3(1,0,0);
	rightdim = length(right);
}

RayHit *Cube::intersect_ray(Ray &r) {

    float tmin,tmax,tymin,tymax,tzmin,tzmax,temp,xflip=1.0f,yflip=1.0f,zflip=1.0f;
    vec3 ent,exit,ent_normal,exit_normal,min_norm,max_norm; 
    RayHit *hit;

    tmin = 100000000;
    tmax = -100000000;

    if (abs(r.dir.x) > 0.0001){

	    tmin = (min.x - r.origin.x) / r.dir.x; 
	    tmax = (max.x - r.origin.x) / r.dir.x;

	    if (tmin > tmax){
	    	temp = tmax;
	    	tmax = tmin;
	    	tmin = temp;

	    	xflip = -1.0f;
	    }

	    min_norm = xflip * xbase;
    	max_norm = xflip * -1.0f * xbase;
	}

    if (abs(r.dir.y) > .00001){
 
	    tymin = (min.y - r.origin.y) / r.dir.y; 
	    tymax = (max.y - r.origin.y) / r.dir.y;

	    if (tymin > tymax){
	    	temp = tymax;
	    	tymax = tymin;
	    	tymin = temp;

	    	yflip = -1.0f;
	    } 
	 
	    if ((tmin > tymax) || (tymin > tmax)){ 
	        return nullptr; 
	    }

	    if (tymin > tmin){ 
	        tmin = tymin;
	        min_norm = yflip * -1.0f * ybase;
	    }

	    if (tymax < tmax){ 
	        tmax = tymax; 
	        max_norm = yflip * ybase;
	    }
	}

	if (abs(r.dir.z) > .00001){
 
	    tzmin = (min.z - r.origin.z) / r.dir.z; 
	    tzmax = (max.z - r.origin.z) / r.dir.z;

	    if (tzmin > tzmax){
	    	temp = tzmax;
	    	tzmax = tzmin;
	    	tzmin = temp;

	    	zflip = -1.0f;
	    }  
	 
	    if ((tmin > tzmax) || (tzmin > tmax)){
	        return nullptr; 
	    }

	    if (tzmin > tmin){ 
	        tmin = tzmin; 
	        min_norm = zflip * -1.0f * zbase;
	    }

	    if (tzmax < tmax){
	        tmax = tzmax;
	        max_norm = yflip * ybase; 
	    }
	}

    if (tmin < 0){
    	return nullptr;
	}

    ent = r.origin + tmin * r.dir;
    exit = r.origin + tmax * r.dir;

    hit = new RayHit(ent,min_norm,tmin,exit,max_norm,tmax,&r,this);

    return hit; 
} 