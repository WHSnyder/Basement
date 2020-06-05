//C++ test for tensorflow from https://stackoverflow.com/questions/56837288/tensorflow-lite-c-api-example-for-inference
//compile command: clang++ -I/home/will/projects/cpprtx/libs/tf_gl/tensorflow/lite/tools/make/downloads/flatbuffers/include -I/home/will/projects/cpprtx/libs/tf_gl -I/home/will/projects/cpprtx/libs/tf_gl/tensorflow/lite/tools/make/downloads/absl ../cpp_tf_test.cpp -o test_model_cpp -ldl -L/home/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/ -ltensorflowlite 
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
#include "StyleTransfer.h"

#define MAC_OPENGL
#include "tensorflow/lite/delegates/gpu/gl_delegate.h"

#include <iostream>
#include <string>


int run_model(){

    StyleTransfer *instance = new StyleTransfer();

    std::string path = instance -> getRenderedStyle(0);



    std::cout << "Result saved at " << path << std::endl;

    delete instance;



    return 0;
}