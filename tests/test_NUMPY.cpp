#include <pybind11/pybind11.h>

#include <iostream>
#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using namespace std;
using namespace glm;

vec3 testVec = vec3(1,0,0);



void dotTest(py::array_t<int, py::array::c_style | py::array::forcecast> array){

}

py::array vecNPGet(){


}










PYBIND11_MODULE(test_NUMPY, m) {
    m.doc() = "Basic glm numpy ops/conversions";
    m.def("Dot test", &dotTest, "Dont numpy array with glm vec");
    m.def("Vec -> NP getter", &vecNPGet, "Convert glm vec to np array and return");
}