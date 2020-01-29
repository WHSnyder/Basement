#include <cstdlib> 
#include <cstdio>
#include <vector>
#include <iostream>

#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

#include "primitives/Obj.h"
#include "Ray.h"
#include "primitives/Sphere.h"
#include "primitives/Plane.h"
#include "primitives/Tri.h"
#include "Scene.h"

#include "CSG.h"

using namespace std;
using namespace glm;
using namespace std::chrono;


void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}


int main(){

	auto start = high_resolution_clock::now(); 

	vec3 pos = vec3(0.0,0.0,1.75);

	vec3 right = vec3(1.0,0.0,0.0);
	vec3 forward = normalize( vec3(0,-2,.5) - pos );
	vec3 up = -1.0f * normalize(cross(right,forward));// vec3(0.0,0.0,1.0);

	if (up.z < 0) up *= -1.0f;

	float dim = 256;
	float plane_dist = 2;
	float plane_width = 3;

	cv::Mat outimg(dim, dim, CV_8UC3, cv::Scalar(100,100,100));
	cv::Mat rawimg = imread("/Users/will/projects/blender/dungeon/textures/MetalSpottyDiscoloration001/Previews/Flat.jpg", cv::IMREAD_COLOR);
	cv::Mat tableimg(rawimg);
	rawimg.convertTo(tableimg, CV_8UC3);
	cv::resize(tableimg, tableimg, cv::Size(1024,1024), 0, 0, cv::INTER_LINEAR);

	vec3 p1 = vec3(-2,-2,-.1);
	vec3 p2 = vec3(2,-2,-.1);
	vec3 p3 = vec3(2,4,-.1);
	vec3 p4 = vec3(-2,4,-.1);

	Plane p = Plane(p1,p2,p3,p4);
	Obj *op = &p;

	Sphere s =  Sphere(vec3(.2,-1.0,1.2), vec3(220,220,220),.4);
	Obj *os = &s;

	Sphere s2 = Sphere(vec3(.2,-1.0,1.2), vec3(200,100,200),.45);
	Obj *os2 = &s2;

	vec3 t0 = vec3(0,-2.7,2.6);
	vec3 t1 = vec3(-2.7,-2.7,-.1);
	vec3 t2 = vec3(2.7,-2.7,-.1); 

	Tri t = Tri(t0,t1,t2);
	Obj *ot = &t;

	/*vec3 t02 = vec3(0,2.9,1.3);
	vec3 t12 = vec3(-1.5,2.5,-.1);
	vec3 t22 = vec3(1.6,2.7,-.1); 

	Tri tr2 = Tri(t22,t12,t02);
	Obj *ot2 = &tr2;*/

	Scene scene;
	scene.add_object(os);
	scene.add_object(ot);
	scene.add_object(os2);
	scene.add_object(op);

	vec3 lightpos = vec3(-.2,-.5,4);
	vec3 lightlook = s.origin;
	vec3 lightdir = lightlook - lightpos;

	scene.add_light(new Light(lightpos, lightdir, vec3(100,20,100)));

	int numhits = 0;

	for (int i = 0; i < dim; i++){

		if (i % 50 == 0 ) cout << "On row " << i << endl;

		for (int j = 0; j < dim; j++){

			float x = .5f * plane_width * (j - dim/2.0f)/(dim/2.0f);
			float y = plane_dist;
			float z = .5f * plane_width * (dim/2.0f - i)/(dim/2.0f);

			vec3 pixelcoord = pos + x * right + y * forward + z * up;

			int hit_index = -1;

			Ray r = Ray(pos, pixelcoord - pos);
			RayHit *hit = scene.intersect_scene(r,&hit_index);

			if (hit != nullptr){
				outimg.at<cv::Vec3b>(i,j) = scene.objects[hit_index] -> shade(hit, &tableimg, &scene);
			}

			delete hit;	
		}
	}

	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<milliseconds>(stop - start); 
	cout << duration.count() << endl; 

	cv::imwrite("output/test.png", outimg);	
	return 0;
}