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

#if __APPLE__
#include "tensorflow/lite/delegates/gpu/metal_delegate.h"
#else
#define MAC_OPENGL
#include "tensorflow/lite/delegates/gpu/gl_delegate.h"
#endif


class StyleTransfer {

public:
    
    StyleTransfer();
    ~StyleTransfer();

    std::string getRenderedStyle(int styleChosen);

private:

    cv::Mat preProcessImage(cv::Mat input);
    std::vector<float> getStyle(int styleVal);
    int fromNameToIndex(std::string stdName, bool isInput, bool isStylePredict) const;

    std::unique_ptr<::tflite::FlatBufferModel> style_predict_model_;
    std::unique_ptr<::tflite::FlatBufferModel> transfer_model_;
    std::unique_ptr<::tflite::Interpreter> style_interpreter_;
    std::unique_ptr<::tflite::Interpreter> transfer_interpreter_;

    TfLiteDelegate *delegate;
};

int run_model();