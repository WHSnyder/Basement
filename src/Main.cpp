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

#include <math.h>

using namespace std;
using namespace glm;

void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}



int main(){


	vec3 right = vec3(1.0,0.0,0.0);
	vec3 forward = vec3(0.0,-1.0,0.0);
	vec3 up = vec3(0.0,0.0,1.0);

	vec3 pos = vec3(0.0,0.0,1.0);

	float dim = 256;
	float plane_dist = 1;
	float plane_width = 2;

	cv::Mat outimg(dim, dim, CV_8UC3, cv::Scalar(0,0,0));
	cv::Mat tableimg = imread("/Users/will/projects/blender/dungeon/textures/MetalSpottyDiscoloration001/Previews/Flat.jpg", cv::IMREAD_COLOR);

	/*
	vec3 p1 = vec3(0,-2,-0.1);
	vec3 p2 = vec3(1,-2,-0.1);
	vec3 p3 = vec3(1,0,-0.1);
	vec3 p4 = vec3(0,0,-0.1);
	*/

	vec3 p1 = vec3(-1,-2,1);
	vec3 p2 = vec3(1,-2,1);
	vec3 p3 = vec3(1,-2,-1);
	vec3 p4 = vec3(-1,-2,-1);

	Plane p = Plane(p1,p2,p3,p4);
	Obj *op = &p;

	Sphere s = Sphere(0,-2,0,1);
	Obj *os = &s;

	cout << "====================================" << endl;
	printVec("xvec",p.xvec);
	printVec("yvec",p.yvec);
	printVec("zvec",p.zvec);
	printVec("origin",p.origin);
	cout << "Length: " << p.length << " Width: " << p.height << endl;
	cout << "====================================" << endl;

	Obj *objects[2];
	objects[0] = op;
	objects[1] = os;

	vec3 lightpos = vec3(1.0,-2.0,1.5);
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
				dotprod = dotprod >= 0 ? 1 : dotprod;

				uint8_t magnitude = (uint8_t) 255 * (1 - dotprod);
				magnitude = magnitude < 40 ? 40 : magnitude;

				vec3 n = 255.0f * normalize(*(rhit -> normal));

				outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(magnitude,magnitude,magnitude);
				//outimg.at<cv::Vec3b>(i,j) = cv::Vec3b((int) abs(n.z),(int) abs(n.y),(int) abs(n.x));

				delete rhit;
				rhit = nullptr;
			}
			/*
			else {

				rhit = op -> intersect_ray(r);

				if (rhit != nullptr){

					vec2 uv = *(rhit -> uv);

					int u = (int) tableimg.rows * uv.x;
					int v = (int) tableimg.cols * uv.y;

					vec3 hit_pos = *(rhit -> worldCoord);

					Ray shadow = Ray(hit_pos, lightpos - hit_pos);
					RayHit *shadow_hit = os -> intersect_ray(shadow);

					//if (shadow_hit != nullptr){
						//cout << "Yolo." << endl;
						//outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
					//}
					//else {
					outimg.at<cv::Vec3b>(i,j) = tableimg.at<cv::Vec3b>(u,v); 
					//}

					if (i <= 450 && i >= 445 && j <= 340 && j >= 335){
						//printVec("hitpos",lightpos - hit_pos);
						//outimg.at<cv::Vec3b>(i,j) = cv::Vec3b(200,200,0);
					}

					delete shadow_hit;
					shadow_hit = nullptr;

					delete rhit;
					rhit = nullptr;
				}
			}
			*/			
		}
	}
	cout << "Num hits: " << numhits << endl;
	cv::imwrite("output/test.png", outimg);	
	return 0;
}