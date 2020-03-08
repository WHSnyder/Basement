#include <pybind11/pybind11.h>

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp> 


using namespace std;

/*
Basic test for sending data back and forth involving use of pybind's OOP functionality.
*/





PYBIND11_MODULE(test_BACKFORTH, m) {
    m.doc() = "Testing basic load cv mat";
    m.def("run_basic", &run_basic, "Run load image test");
}