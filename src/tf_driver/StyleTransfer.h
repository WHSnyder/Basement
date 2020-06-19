/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~ Copyright 2019 Adobe
 ~
 ~ Licensed under the Apache License, Version 2.0 (the "License");
 ~ you may not use this file except in compliance with the License.
 ~ You may obtain a copy of the License at
 ~
 ~     http://www.apache.org/licenses/LICENSE-2.0
 ~
 ~ Unless required by applicable law or agreed to in writing, software
 ~ distributed under the License is distributed on an "AS IS" BASIS,
 ~ WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ~ See the License for the specific language governing permissions and
 ~ limitations under the License.
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once

#include "tensorflow/lite/c/c_api.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"

#include "opencv2/opencv.hpp"
#include <string>


#if __APPLE__
#include "tensorflow/lite/delegates/gpu/metal_delegate.h"
#else
#define MAC_OPENGL
#include "tensorflow/lite/delegates/gpu/gl_delegate.h"
#endif


class StyleTransfer {

public:
    
    StyleTransfer(unsigned int ssboOut = 10000, unsigned int ssboIn = 10000);
    ~StyleTransfer();

    void setStyle(int styleVal);
    int execute();
    int prime();

private:

    cv::Mat preProcessImage(cv::Mat input);
    std::vector<float> styleEncoding;

    int fromNameToIndex(std::string stdName, bool isInput, bool isStylePredict) const;

    std::unique_ptr<::tflite::FlatBufferModel> predictorModel;
    std::unique_ptr<::tflite::FlatBufferModel> transfererModel;
    std::unique_ptr<::tflite::Interpreter> styleInterpreter;
    std::unique_ptr<::tflite::Interpreter> transferInterpreter;

    TfLiteDelegate *delegate;
};

int run_model(unsigned int ssbo=10000, unsigned int ssboIn=10000);