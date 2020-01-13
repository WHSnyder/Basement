#include <cstdlib> 
#include <cstdio>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

//#include <glm/ext/vector_float2.hpp>
//#include <glm/ext/vector_float3.hpp>

#include <opencv2/opencv.hpp> 


using namespace std;
using namespace glm;


vec3 raycast(vec3 pixelcoord){

	vec3 origin = vec3(0.0,0.0,0.0);
	

	return origin;

}





int main(){

	int dim = 256;
	int plane_dist = 1;
	int plane_width = 1;

	cv::Mat outimg(256, 256, CV_8UC1, cv::Scalar(0));
	uint8_t *image_data = outimg.data;
	int _stride = outimg.step;


	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			
			float x = .5 * plane_width * (j - dim/2)/(dim/2);
			float y = .5 * plane_width * (dim - i - dim/2)/(dim/2);
			float z = plane_dist;

			vec3 pixelcoord = vec3(x,y,z);


		}
	}

	cv::imwrite("test.jpg", outimg);	
}




