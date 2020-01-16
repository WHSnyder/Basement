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



using namespace std;
using namespace glm;



int main(){

	int dim = 256;
	int plane_dist = 1;
	int plane_width = 1;

	cv::Mat outimg(256, 256, CV_8UC1, cv::Scalar(0));
	uint8_t *image_data = outimg.data;
	int _stride = outimg.step;

	Sphere s = Sphere(0.0,.5,-3,2);


	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			
			float x = .5 * plane_width * (j - dim/2)/(dim/2);
			float y = .5 * plane_width * (dim - i - dim/2)/(dim/2);
			float z = -plane_dist;

			vec3 pixelcoord = vec3(x,y,z);

			Ray r = Ray(vec3(0.0,0.0,0.0), pixelcoord);

			vec3 hit = s.intersect_ray(r);

			if (hit.z > .5){
				image_data[_stride * i + j] = 254;
			}
		}
	}

	cv::imwrite("output/test.jpg", outimg);	
	return 0;
}




