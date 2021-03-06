//C++ test for tensorflow from https://stackoverflow.com/questions/56837288/tensorflow-lite-c-api-example-for-inference

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
#include "StyleTransfer.h"

#include <iostream>
#include <string>


int run_model(unsigned int ssbo, unsigned int ssbo_in){

    StyleTransfer *instance = new StyleTransfer(ssbo, ssbo_in);
    instance -> setStyle(0);
    instance -> prime();
    instance -> execute();

    std::string path = instance -> getRenderedStyle(0);

    delete instance;

    return 0;
}