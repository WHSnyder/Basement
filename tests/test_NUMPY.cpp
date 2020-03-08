#include <pybind11/pybind11.h>

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp> 


using namespace std;


int num_images = 0;

int vec3[3] = {1,0,0};


class MyVec {

public:

	int x,y,z;

	MyVec(int x_,int y_, int z_){
		x = x_;
		y = y_;
		z = z_;
	}

	int dot(MyVec other){
		return x*other.x + y*other.y + z*other.z;
	}
}




PYBIND11_MODULE(test_NUMPY, m) {
    m.doc() = "Testing basic load cv mat";
    m.def("run_basic", &run_basic, "Run load image test");
}