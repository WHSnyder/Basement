#include <cstdlib> 
#include <cstdio>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <opencv2/opencv.hpp> 

using namespace std;
using namespace cv;


int main(){

	int dim = 256;
	int plane_dist = 1;
	float plane_width = .5;

	Mat outimg(256, 256, CV_8UC1, Scalar(0));
	uint8_t *image_data = outimg.data;
	int _stride = outimg.step;

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			if (i % 4 == 0 || j % 4 == 0){
				image_data[i * _stride + j] = 255;
			}
		}
	}

	imwrite("test.jpg", outimg);	
}




