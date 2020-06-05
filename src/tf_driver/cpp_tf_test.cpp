//C++ test for tensorflow from https://stackoverflow.com/questions/56837288/tensorflow-lite-c-api-example-for-inference
//compile command: clang++ -I/home/will/projects/cpprtx/libs/tf_gl/tensorflow/lite/tools/make/downloads/flatbuffers/include -I/home/will/projects/cpprtx/libs/tf_gl -I/home/will/projects/cpprtx/libs/tf_gl/tensorflow/lite/tools/make/downloads/absl ../cpp_tf_test.cpp -o test_model_cpp -ldl -L/home/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/ -ltensorflowlite 
#include <iostream>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
#include "StyleTransfer.h"
#include <string>

#define MAC_OPENGL
#include "tensorflow/lite/delegates/gpu/gl_delegate.h"




/*
int main(){

    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/will/projects/cpprtx/libs/tf_models/test_tfconv.tflite");

    if(!model){
        printf("Failed to mmap model\n");
        exit(0);
    }

    tflite::ops::builtin::BuiltinOpResolver resolver;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);

    // Resize input tensors, if desired.
    interpreter->AllocateTensors();

    float* input = interpreter->typed_input_tensor<float>(0);
    // Dummy input for testing
    *input = 4.0;

    interpreter->Invoke();

    float* output = interpreter->typed_output_tensor<float>(0);

    printf("Result is: %f\n", *output);

    return 0;
}*/


int run_model(){

    StyleTransfer *instance = new StyleTransfer();

    std::string path = instance -> getRenderedStyle(0);



    std::cout << "Result saved at " << path << std::endl;

    delete instance;



    return 0;
}