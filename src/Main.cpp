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

	int dim = 256;
	int plane_dist = 1;
	float plane_width = 2;

	cv::Mat outimg(256, 256, CV_8UC3, cv::Scalar(0,0,0));
	uint8_t *image_data = outimg.data;
	int _stride = outimg.step;

	cv::Mat tableimg = imread("/Users/will/projects/blender/dungeon/textures/MetalSpottyDiscoloration001/Previews/Flat.jpg", cv::IMREAD_COLOR);
	cv::resize(tableimg, tableimg, cv::Size(512,512), 0, 0, cv::INTER_LINEAR);

	vec3 p1 = vec3(-2,0.5,-3);
	vec3 p2 = vec3(2,0.5,-3);
	vec3 p3 = vec3(2,0.1,-1);
	vec3 p4 = vec3(-2,0.1,-1);

	Plane p = Plane(p1,p2,p3,p4);
	Obj *op = &p;

	Sphere s = Sphere(0.0,-.1,-3,1);
	Obj *os = &s;

	vec3 lightpos = vec3(0.0,1.0,-2.5);
	vec3 lightlook = vec3(0.0,.5,-3);
	vec3 lightdir = normalize(lightlook - lightpos);

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			
			float x = .5 * plane_width * (j - dim/2)/(dim/2);
			float y = .5 * plane_width * (dim - i - dim/2)/(dim/2);
			float z = -plane_dist;

			vec3 pixelcoord = vec3(x,y,z);

			Ray r = Ray(vec3(0.0,0.0,0.0), pixelcoord);

			vec3 hit = os->intersect_ray(r);

			if (hit.z > .01){

				float dotprod = dot(hit,lightdir);

				if (dotprod > -.01){
					continue;
				}
				int magnitude = (int) 255 * (1 - dotprod);
				outimg.at<cv::Vec3b>(cv::Point(j,i)) = cv::Vec3b(magnitude,0,0);
				//image_data[_stride * i + j] = (uint8_t) magnitude;
			}
			else {

				hit = op -> intersect_ray(r);

				if (hit.z > .01){

					int u = (int) 512 * hit.x;
					int v = (int) 512 * hit.y;

					//cout << "hi" << endl;

					outimg.at<cv::Vec3b>(cv::Point(j,i)) = tableimg.at<cv::Vec3b>(cv::Point(u,v)); 
					//image_data[_stride * i + j] = ;
				}
			}
		}
	}

	cv::imwrite("output/test.jpg", outimg);	
	return 0;
}




