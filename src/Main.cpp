#include <cstdlib> 
#include <cstdio>
#include <iostream>
#include <vector>

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

#include <math.h>

using namespace std;
using namespace glm;



int main(){

	int dim = 512;
	int plane_dist = 1;
	float plane_width = 1;

	cv::Mat outimg(dim, dim, CV_8UC3, cv::Scalar(0,0,0));
	cv::Mat tableimg = imread("/Users/will/projects/blender/dungeon/textures/MetalSpottyDiscoloration001/Previews/Flat.jpg", cv::IMREAD_COLOR);
	cv::resize(tableimg, tableimg, cv::Size(512,512), 0, 0, cv::INTER_LINEAR);

	vec3 p1 = vec3(-2,0.6,-3);
	vec3 p2 = vec3(4,0.6,-3);
	vec3 p3 = vec3(4,0.1,0);
	vec3 p4 = vec3(-2,0.1,0);

	Plane p = Plane(p1,p2,p3,p4);
	Obj *op = &p;

	Sphere s = Sphere(1.0,-.1,-3,1);
	Obj *os = &s;

	Obj *objects[2];
	objects[0] = op;
	objects[1] = os;

	vec3 lightpos = vec3(2.0,1.0,-2);
	vec3 lightlook = vec3(0.0,-.1,-3);
	vec3 lightdir = normalize(lightlook - lightpos);

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			
			float x = plane_width * (j - dim/2)/(dim/2);
			float y = plane_width * (dim - i - dim/2)/(dim/2);
			float z = -plane_dist;

			vec3 pixelcoord = vec3(x,y,z);

			Ray r = Ray(vec3(0.0,0.0,0.0), pixelcoord);
			RayHit* rhit = os->intersect_ray(r);


			if (rhit != nullptr){

				float dotprod = dot(rhit -> normal,lightdir);
				dotprod = dotprod > 0 ? 1 : dotprod;

				uint8_t magnitude = (uint8_t) 255 * (1 - dotprod);
				magnitude = magnitude < 40 ? 40 : magnitude;

				vec3 n = 255.0f * normalize(rhit -> normal);

				outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(magnitude,magnitude,magnitude);

				delete rhit;
				rhit = nullptr;
			}

			else {

				rhit = op -> intersect_ray(r);

				if (rhit != nullptr){

					vec2 uv = rhit -> uv;

					int u = (int) 512 * uv.x;
					int v = (int) 512 * uv.y;

					Ray shadow = Ray(rhit -> worldCoord, lightpos - rhit -> worldCoord);
					RayHit *shadow_hit = os -> intersect_ray(shadow);

					if (shadow_hit != nullptr){
						outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
					}
					else {
						outimg.at<cv::Vec3b>(i,j) = tableimg.at<cv::Vec3b>(u,v); 
						if (j > 20 && j < 25 && i <= 450 && i >= 445){
							outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,240); 
							cout << u << "  " << v << endl;
						}
					}

					delete shadow_hit;
					shadow_hit = nullptr;

					delete rhit;
					rhit = nullptr;
				}
			}			
		}
	}
	cv::imwrite("output/test.jpg", outimg);	
	return 0;
}