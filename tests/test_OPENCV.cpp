#include <pybind11/pybind11.h>

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp> 


using namespace std;


int num_images = 0;


void run_basic() {
	string filepath = string("/Users/will/projects/cpprtx/assets/images/grass.jpg");
	cv::Mat img = imread(filepath, cv::IMREAD_COLOR);
	cv::resize(img, img, cv::Size(1024,1024), 0, 0, cv::INTER_LINEAR);
	cout << img.cols << endl;
}


PYBIND11_MODULE(test_OPENCV, m) {
    m.doc() = "Testing basic load cv mat";
    m.def("run_basic", &run_basic, "Run load image test");
}