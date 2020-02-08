#include <cstdlib> 
#include <cstdio>
#include <vector>
#include <iostream>
#include <pthread.h>
//#include <format>

#ifndef cvinc
#define cvinc
#include <opencv2/opencv.hpp> 
#endif

#include "Scene.h"
#include "primitives/Obj.h"
#include "CSG.h"

using namespace std;
using namespace glm;
using namespace std::chrono;

//#define FRAMES 60


void printVec(string name,vec3 v){
	cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}


string arg_to_string(char* a) { 
    int i; 
    string s = ""; 
    for (i = 0; i < 1000; i++) { 
    	if (a[i] == 0) break;
        s = s + a[i]; 
    } 
    return s;
} 


struct thread_input {

	cv::Mat *write_img;
	int start_index;
	int end_index;
	Scene *scene;
	cv::Mat *tableimg;
	int bounces;
};


void *trace_pixels(void *thread_args){

	thread_input *input = (thread_input *) thread_args;

	cv::Mat *write_img = input -> write_img;
	cv::Mat *tableimg = input -> tableimg;

	unsigned char *output = (unsigned char*)(write_img->data);

	Scene *scene = input -> scene; 

	int start_index = input->start_index, end_index = input->end_index;
	int cols = write_img->cols, rows = write_img->rows, index,hit_index,i,j;
	
	float plane_dist = 2, plane_width = 3,x,y,z;

	vec3 pixelcoord;
	RayHit *hit;
	Ray *r;

	vec3 pos = scene -> view -> pos, up = scene -> view -> up;
	vec3 right = scene -> view -> right, forward = scene -> view -> forward;

	cv::Vec3b color;


	for (int p = start_index; p < end_index; p++){

		i = p / cols;
		j = p % cols;

		index = 3 * p;

		x = .5f * plane_width * (j - cols*.5f)/(cols*.5f);
		y = plane_dist;
		z = .5f * plane_width * (rows*.5f - i)/(rows*.5f);

		pixelcoord = pos + x * right + y * forward + z * up;

		hit_index = -1;

		r = new Ray(pos, pixelcoord - pos);
		hit = scene -> intersect_scene(*r, &hit_index);

		if (hit != nullptr){

			Obj *obj_hit = hit -> object_hit;
			color = obj_hit -> shade(hit, tableimg, scene, input -> bounces);
			
			output[index] = color[0];
			output[index + 1] = color[1];
			output[index + 2] = color[2];
		}
		else {

			output[index] = 30;
			output[index + 1] = 30;
			output[index + 2] = 30;
		}

		delete hit;	
	}

	pthread_exit(NULL);
}



int main(int argc, char **argv){

	vec3 pos = vec3(0.0,0.0,1.75);

	vec3 right = vec3(1.0,0.0,0.0);
	vec3 forward = normalize( vec3(0,-2,.5) - pos );
	vec3 up = -1.0f * normalize(cross(right,forward));

	if (up.z < 0) up *= -1.0f;

	View view = View(forward,up,right,pos);

	string::size_type sz;
  	float dim = stoi(arg_to_string(argv[1]),&sz);
  	int bounces = 2;//
  	int FRAMES = stoi(arg_to_string(argv[3]),&sz), num_threads = 12;
	float plane_dist = 2, plane_width = 3,x,y,z;

	cv::Mat outimg(dim, dim, CV_8UC3, cv::Scalar(10,10,10));
	cv::Mat outputimg(1024, 1024, CV_8UC3, cv::Scalar(10,10,10));
	cv::Mat rawimg = imread("/Users/will/projects/blender/dungeon/textures/sewer2.png", cv::IMREAD_COLOR);
	cv::Mat tableimg(rawimg);
	rawimg.convertTo(tableimg, CV_8UC3);
	cv::resize(tableimg, tableimg, cv::Size(2048,2048), 0, 0, cv::INTER_LINEAR);

	vec3 p1 = vec3(-2,-2,-.1);
	vec3 p2 = vec3(2,-2,-.1);
	vec3 p3 = vec3(2,4,-.1);
	vec3 p4 = vec3(-2,4,-.1);

	Plane p = Plane(p1,p2,p3,p4);
	Obj *op = &p;

	vec3 p12 = vec3(-1,2,.1);
	vec3 p22 = vec3(-1,-2,.1);
	vec3 p32 = vec3(-1,-2,1.9);
	vec3 p42 = vec3(-1,2,1.9);

	Plane _p1 = Plane(p12,p22,p32,p42);
	Obj *op1 = &_p1;
	_p1.shader = &shade_reflective;

	//static sphere
	Sphere s0 =  Sphere(vec3(-.2,-1.1,1.0), vec3(240,20,20),.4);
	Obj *os0 = &s0;

	Sphere s1 =  Sphere(vec3(-.2,-1.1,1.4), vec3(40,40,140),.4);
	Obj *os1 = &s1;

	//moving sphere
	Sphere s2 = Sphere(vec3(-.19,-1.1,1.7), vec3(250,170,170),.1);
	Obj *os2 = &s2;
	//s1.shader = &shade_reflective;



	vec3 lb = vec3(-.1,-.6,1.2);
	Cube c0 = Cube(lb,lb + 4.0f * vec3(.1,-.1,.1));
	Obj *oc0 = &c0;

	vec3 t0 = vec3(0,-2.7,2.6);
	vec3 t1 = vec3(-2.7,-2.7,-.1);
	vec3 t2 = vec3(2.7,-2.7,-.1); 

	Tri t = Tri(t0,t1,t2);
	Obj *ot = &t;


	Scene scene = Scene(&view);

	vec3 lightpos = vec3(0,-.5,3);
	vec3 lightlook = vec3(0,-1.0,1.2);
	vec3 lightdir = lightlook - lightpos;

	scene.add_light(new Light(lightpos, lightdir, vec3(100,20,100)));

	CSG sphere_0 = CSG(os0);
	CSG sphere_1 = CSG(os1);
	CSG sphere_2 = CSG(os2);
	CSG cube_0 = CSG(oc0);

	CSG *combo;

	string csgop = arg_to_string(argv[2]);
	if (csgop.compare("union") == 0) combo = sphere_0 || sphere_1;
	else if (csgop.compare("intx") == 0) combo = sphere_0 && sphere_1;
	else combo = sphere_0 - sphere_1;
	
	CSG planecsg = CSG(op);
	CSG planecsg2 = CSG(op1);
	CSG tricsg = CSG(ot);

	scene.add_csg(combo);
	scene.add_csg(&planecsg);
	scene.add_csg(&planecsg2);
	scene.add_csg(&tricsg);

	int limit = outimg.rows * outimg.cols,i, start_index, end_index, rc;

	auto start = high_resolution_clock::now(); 

    pthread_t threads[num_threads];
    pthread_attr_t attr;
    void *status;

    std::string strs[3] = {"output/output0.png","output/output1.png","output/output2.png"};

    scene.test_sphere = &s1;
    scene.test_plane = &p;
    scene.static_sphere = &s0;

    for (int f = 0; f < FRAMES; f++){

    	scene.update_physics();

	    pthread_attr_init(&attr);
	    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	    for (i = 0; i < num_threads; i++) {
	    	
	    	thread_input *input = new thread_input();
	    	
	    	input -> start_index = i * limit / num_threads;
	    	input -> end_index = (i + 1) * limit / num_threads;
	    	input -> write_img = &outimg;
	    	input -> scene = &scene;
	    	input -> tableimg = &tableimg;
	    	input -> bounces = bounces;

	    	rc = pthread_create(&threads[i], &attr, trace_pixels, (void *) input); 
	    }

	    pthread_attr_destroy(&attr);

	    for (i = 0; i < num_threads; i++) {
	        
	        rc = pthread_join(threads[i], &status);
	        
	        if (rc) {
	            cout << "Join error!" << rc << endl;
	            exit(-1);
	        }
	    }

		//ffmpeg -pattern_type glob -i './output/output_*.png' -vcodec libx264 -vf scale=640:-2,format=yuv420p ./output/outvid.mp4
	
	    cv::resize(outimg, outputimg, cv::Size(1024,1024), 0, 0, cv::INTER_LINEAR);
		cv::imwrite("output/output_" + std::to_string(100+f) + ".png" , outputimg);
	    
		cout << "Writing " << f << endl;
	}

	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<milliseconds>(stop - start); 
	cout << "Elapsed: " << duration.count() << endl; 

	return 0;
}