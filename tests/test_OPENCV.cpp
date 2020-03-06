#include <pybind11/pybind11.h>

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp> 



using namespace std;

void run_test(int argc, char **argv){

	string filepath = string("/Users/will/projects/cpprtx/assets/images/grass.jpg");
	cv::Mat img = imread(filepath, cv::IMREAD_COLOR);
	cv::resize(img, img, cv::Size(1024,1024), 0, 0, cv::INTER_LINEAR);

	cout << "Yolo" << endl;
}


PYBIND11_MODULE(test_OPENCV, m) {
    m.doc() = "Testing basic load cv mat";
    m.def("runtest", &runtest, "Run load image test");
}
