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



void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}


int main(){


	vec3 right = vec3(1.0,0.0,0.0);
	vec3 forward = vec3(0.0,-1.0,0.0);
	vec3 up = -1.0f * normalize(cross(right,forward));// vec3(0.0,0.0,1.0);

	vec3 pos = vec3(0.0,0.0,0.5);

	float dim = 512;
	float plane_dist = 2;
	float plane_width = 3;

	cv::Mat outimg(dim, dim, CV_8UC3, cv::Scalar(100,100,100));
	cv::Mat rawimg = imread("/Users/will/projects/blender/dungeon/textures/MetalSpottyDiscoloration001/Previews/Flat.jpg", cv::IMREAD_COLOR);
	cv::Mat tableimg(rawimg);
	rawimg.convertTo(tableimg, CV_8UC3);
	cv::resize(tableimg, tableimg, cv::Size(1024,1024), 0, 0, cv::INTER_LINEAR);


	vec3 p1 = vec3(-.6,-2,-.1);
	vec3 p2 = vec3(.6,-2,-.1);
	vec3 p3 = vec3(.6,0,-.2);
	vec3 p4 = vec3(-.6,0,-.2);

	Plane p = Plane(p1,p2,p3,p4);
	Obj *op = &p;

	Sphere s = Sphere(0,-1.5,.7,.3);
	Obj *os = &s;

	vec3 t1 = vec3(-.6,-2,-.1);
	vec3 t2 = vec3(.6,-2,-.1);
	vec3 t3 = vec3(0,-2,1.0);

	Tri t = Tri(t1,t2,t3);
	Obj *ot = &t;

	Obj *objects[2];
	objects[0] = op;
	objects[1] = os;

	vec3 lightpos = vec3(-.5,-1.25,4);
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

				float dotprod = dot(*(rhit -> normal),lightdir);
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

					vec2 uv = *(rhit -> uv);

					int u = (int) (tableimg.rows - 1) * uv.x;
					int v = (int) (tableimg.cols - 1 )* uv.y;

					vec3 hit_pos = *(rhit -> worldCoord);

					Ray shadow = Ray(hit_pos, lightpos - hit_pos);
					RayHit *shadow_hit = os -> intersect_ray(shadow);


					if (shadow_hit != nullptr){
						outimg.at<cv::Vec3b>(i,j) = tableimg.at<cv::Vec3b>(u,v)/2;// cv::Vec3b(0,0,0);
					}
					else {
						float dotprod = -1.0f * dot(*(rhit -> normal),lightdir);
						dotprod = dotprod < 0 ? 0 : dotprod;

						outimg.at<cv::Vec3b>(i,j) = dotprod * tableimg.at<cv::Vec3b>(u,v); 
					}


					delete shadow_hit;
					shadow_hit = nullptr;

					delete rhit;
					rhit = nullptr;
				}

				rhit = ot -> intersect_ray(r);
				if (rhit != nullptr){
					outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(1,200,1);
					delete rhit;
				}
			}	
		}
	}
	cout << "Num hits: " << numhits << endl;
	cv::imwrite("output/test.png", outimg);	
	return 0;
}