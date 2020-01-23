#include <cstdlib> 
#include <cstdio>
#include <vector>
#include <iostream>

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif

#include <opencv2/opencv.hpp> 

#include "Ray.h"
#include "Obj.h"
#include "Sphere.h"
#include "Plane.h"
#include "Tri.h"

#include <math.h>

using namespace std;
using namespace glm;
using namespace std::chrono;



void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}


vec3 reflect(vec3 normal, vec3 direction){
	return -2.0f * dot(normal,direction) * normal + direction;
}

/*
RayHit *intersect_scene(Obj *objs, Ray& r){
	int i = 0, min_dist = 1000;
	RayHit hit = nullptr, cur = nullptr;
	
	while (objs[i] != nullptr){

		cur = objs[i] -> intersect_ray(r);

		if (){

		}
	}
	return nullptr;
}
*/



int main(){

	auto start = high_resolution_clock::now(); 

	vec3 pos = vec3(0.0,-.5,1.75);

	vec3 right = vec3(1.0,0.0,0.0);
	vec3 forward = normalize( vec3(0,-2,.5) - pos );
	vec3 up = -1.0f * normalize(cross(right,forward));// vec3(0.0,0.0,1.0);

	if (up.z < 0) up *= -1.0f;

	float dim = 512;
	float plane_dist = 2;
	float plane_width = 3;

	cv::Mat outimg(dim, dim, CV_8UC3, cv::Scalar(100,100,100));
	cv::Mat rawimg = imread("/Users/will/projects/blender/dungeon/textures/MetalSpottyDiscoloration001/Previews/Flat.jpg", cv::IMREAD_COLOR);
	cv::Mat tableimg(rawimg);
	rawimg.convertTo(tableimg, CV_8UC3);
	cv::resize(tableimg, tableimg, cv::Size(1024,1024), 0, 0, cv::INTER_LINEAR);

	vec3 p1 = vec3(-1,-2,-.1);
	vec3 p2 = vec3(1,-2,-.1);
	vec3 p3 = vec3(1,10,-.1);
	vec3 p4 = vec3(-1,10,-.1);

	Plane p = Plane(p1,p2,p3,p4);
	Obj *op = &p;

	Sphere s = Sphere(.25,-1.5,1,.3);
	Obj *os = &s;

	vec3 t0 = vec3(0,-2,1.5);
	vec3 t1 = vec3(-1,-2,-.1);
	vec3 t2 = vec3(1,-2,-.1); 

	Tri t = Tri(t0,t1,t2);
	Obj *ot = &t;

	vec3 lightpos = vec3(-.2,-1.5,4);
	vec3 lightlook = s.origin;
	vec3 lightdir = normalize(lightlook - lightpos);

	int numhits = 0;

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			
			float x = .5f * plane_width * (j - dim/2.0f)/(dim/2.0f);
			float y = plane_dist;
			float z = .5f * plane_width * (dim/2.0f - i)/(dim/2.0f);

			vec3 pixelcoord = pos + x * right + y * forward + z * up;

			Ray r = Ray(pos, pixelcoord - pos);
			RayHit* rhit = os->intersect_ray(r);

			if (rhit != nullptr){

				numhits++;

				float dotprod = dot(*rhit -> normal,lightdir);
				dotprod = dotprod > -0.1000001 ? 1 : dotprod;

				uint8_t magnitude = (uint8_t) 255 * (1 - dotprod);
				magnitude = magnitude < 40 ? 40 : magnitude;

				outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(magnitude,magnitude,magnitude);

				delete rhit;
				rhit = nullptr;
			}
			
			else {

				RayHit* rhit = op -> intersect_ray(r);

				if (rhit != nullptr){

					vec2 uv = *rhit -> uv;

					int u = (int) (tableimg.rows - 1) * uv.x;
					int v = (int) (tableimg.cols - 1 )* uv.y;

					vec3 hit_pos = *rhit -> worldCoord;

					Ray shadow = Ray(hit_pos, lightpos - hit_pos);
					RayHit *shadow_hit = os -> intersect_ray(shadow);

					if (shadow_hit != nullptr){
						outimg.at<cv::Vec3b>(i,j) = tableimg.at<cv::Vec3b>(u,v)/2;
					}
					else {
						float dotprod = -1.0f * dot(*rhit -> normal,lightdir);
						dotprod = dotprod < 0 ? 0 : dotprod;
						outimg.at<cv::Vec3b>(i,j) = dotprod * tableimg.at<cv::Vec3b>(u,v); 
					}

					delete shadow_hit;
					shadow_hit = nullptr;

					delete rhit;
					rhit = nullptr;
				}
				else {
					rhit = ot -> intersect_ray(r);

					if (rhit != nullptr){

						Ray reflection = Ray(*rhit->worldCoord,reflect(*rhit->normal,r.dir));
						RayHit *reflect_hit = op -> intersect_ray(reflection);

						cv::Vec3b col = cv::Vec3b(50,50,50);

						if (reflect_hit != nullptr){

							vec2 uv = *reflect_hit -> uv;

							int u = (int) (tableimg.rows - 1) * uv.x;
							int v = (int) (tableimg.cols - 1 )* uv.y;

							col = cv::Vec3b(200,0,0); 
						}

						reflect_hit = os -> intersect_ray(reflection);
						if (reflect_hit != nullptr){
							col = cv::Vec3b(0,0,200);
						}

						outimg.at<cv::Vec3b>(i,j) = col;

						delete reflect_hit;
					}
				}
				delete rhit;
			}	
		}
	}
	auto stop = high_resolution_clock::now(); 

	auto duration = duration_cast<milliseconds>(stop - start); 
	cout << duration.count() << endl; 

	cv::imwrite("output/test.png", outimg);	
	return 0;
}