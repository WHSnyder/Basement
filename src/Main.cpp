#include <cstdlib> 
#include <cstdio>
#include <vector>
#include <iostream>
#include <pthread.h>

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


int arg_to_num(char* a) { 
    int i; 
    string s = ""; 
    for (i = 0; i < 1000; i++) { 
    	if (a[i] == 0) break;
        s = s + a[i]; 
    } 
    string::size_type sz;   // alias of size_t
  	return stoi(s,&sz);
} 



/*

struct thread_input {

	cv::Mat *write_img;
	int start_index;
	int end_index;
	Scene *scene;
};




void *trace_pixels(void *thread_args){

	thread_input *input = (thread_input *) thread_args;

	cv::Mat *write_img = input -> write_img;
	unsigned char *output = (unsigned char*)(write_img->data);

	int start_index = input -> start_index;
	int end_index = input -> end_index;
	Scene *scene = input -> scene; 

	int cols = write_img->cols,row,col,index,hit_index;
	float x,y,z;
	vec3 pixelcoord;
	RayHit *hit;
	Ray r;

	vec3 pos = scene -> view.pos;
	vec3 up = scene -> view.up;
	vec3 right = scene -> view.right;
	vec3 forward = scene -> view.forward;

	cv::Vec3b color;

	for (int i = start_index; i < end_index; i++){

		row = i / cols;
		col = i % cols;
		index = row * i + col;
		x = .5f * plane_width * (col - dim/2.0f)/(dim/2.0f);
		y = plane_dist;
		z = .5f * plane_width * (dim/2.0f - row)/(dim/2.0f);

		pixelcoord = pos + x * right + y * forward + z * up;

		hit_index = -1;

		r = Ray(pos, pixelcoord - pos);
		*hit = scene.intersect_scene(r, &hit_index);

		if (hit != nullptr){
			Obj *obj_hit = hit -> object_hit;
			color = obj_hit -> shade(hit, &tableimg, &scene);
			
			//outimg.at<cv::Vec3b>(row,col) = obj_hit -> shade(hit, &tableimg, &scene);

			output[index] = color[0];
			output[index + 1] = color[1];
			output[index + 2] = color[2];
		}

		delete hit;	
	}
}


*/


int main(int argc, char **argv){

	auto start = high_resolution_clock::now(); 

	vec3 pos = vec3(0.0,0.0,1.75);

	vec3 right = vec3(1.0,0.0,0.0);
	vec3 forward = normalize( vec3(0,-2,.5) - pos );
	vec3 up = -1.0f * normalize(cross(right,forward));

	if (up.z < 0) up *= -1.0f;

	View view = View(forward,up,right,pos);

	float dim = arg_to_num(argv[1]);
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

	Sphere s =  Sphere(vec3(0,-1.1,1.2), vec3(220,220,220),.4);
	Obj *os = &s;

	Sphere s2 = Sphere(vec3(.2,-.9,1.1), vec3(220,50,180),.33);
	Obj *os2 = &s2;

	vec3 t0 = vec3(0,-2.7,2.6);
	vec3 t1 = vec3(-2.7,-2.7,-.1);
	vec3 t2 = vec3(2.7,-2.7,-.1); 

	Tri t = Tri(t0,t1,t2);
	Obj *ot = &t;

	Scene scene = Scene(&view);
	scene.add_object(os);
	scene.add_object(ot);
	scene.add_object(os2);
	scene.add_object(op);

	vec3 lightpos = vec3(0,-.5,3);
	vec3 lightlook = vec3(0,-1.0,1.2);
	vec3 lightdir = lightlook - lightpos;

	scene.add_light(new Light(lightpos, lightdir, vec3(100,20,100)));

	CSG sphere_0 = CSG(os);
	CSG sphere_1 = CSG(os2);

	CSG *combo = sphere_0 - sphere_1;
	CSG planecsg = CSG(op);
	CSG tricsg = CSG(ot);

	scene.add_csg(combo);
	scene.add_csg(&planecsg);
	scene.add_csg(&tricsg);


	//int num_threads = 4, range = outimg.rows * outimg.cols, div = range/4;

	//for (int i = 0; i < 4; i )


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
				Obj *obj_hit = hit -> object_hit;
				outimg.at<cv::Vec3b>(i,j) = obj_hit -> shade(hit, &tableimg, &scene);
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