#pragma once

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"

#define MAC_OPENGL
#include "tensorflow/lite/delegates/gpu/gl_delegate.h"

#include <opencv2/opencv.hpp>
#include <stdlib.h>

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