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

#include "StyleTransfer.h"
#include <iostream>
#include <string>

std::string MODEL_PATH = "/home/will/projects/cpprtx/libs/tf_models/magenta_models/";
std::string APP_PATH = "/home/will/Desktop/";
std::string ZION = APP_PATH + "grad.jpg";
std::string INPUT_IMAGE = APP_PATH + "gate.jpg";
std::string style_predict_model = MODEL_PATH + "arb_style_predict.tflite";
std::string style_transfer_model = MODEL_PATH + "arb_style_transform.tflite";
std::string LASSEN = APP_PATH + "scream.jpg";
std::string GRAND_CANYON = APP_PATH + "starry.jpg";


void printVector(std::vector<int> const &a) {
    std::cout << "Input indicies: ";
    for(int i=0; i < a.size(); i++)
       std::cout << a.at(i) << ' ';
    std::cout << std::endl;
}


StyleTransfer::StyleTransfer() {

    // Spin up the interpreter
    style_predict_model_ = ::tflite::FlatBufferModel::BuildFromFile(style_predict_model.c_str());
    transfer_model_ = ::tflite::FlatBufferModel::BuildFromFile(style_transfer_model.c_str());
    ::tflite::ops::builtin::BuiltinOpResolver resolver;
    ::tflite::InterpreterBuilder style_builder(*style_predict_model_, resolver);
    ::tflite::InterpreterBuilder transform_builder(*transfer_model_, resolver);

    if (style_builder(&style_interpreter_) != kTfLiteOk) {
        std::cout << "Error with style interpreter" << std::endl;
    }

    if (transform_builder(&transfer_interpreter_) != kTfLiteOk) {
        std::cout << "Error with transfer interpreter" << std::endl;
    }

    std::cout << "Bind delegate" << std::endl;

    // NEW: Prepare GPU delegate.
    delegate = TfLiteGpuDelegateCreate(/*default options=*/nullptr);
    if (style_interpreter_ -> ModifyGraphWithDelegate(delegate) != kTfLiteOk){
        std::cout << "BIG FAIL" << std::endl;  
    } 
}

/*
 * This will run the inference on both, so that we can get the transformed image.
 *
 * Input: An OpenCV Mat
 * Input: A pre-defined style
 *
 * Output: A String going to where the stored image is on private storage
 */
std::string StyleTransfer::getRenderedStyle(int styleChosen) {

    std::cout << "Getting rendered style" << std::endl;

    // Predict the style
    // Resize the image to the shape and do pre-processing
    // Do the style transfert
    // Do the post-processing to get this image back into a jpeg format

    // Do the style transfer code
    std::vector<float> styleVec = getStyle(styleChosen);

    if(styleVec.size() > 0) {

        cv::Mat image = cv::imread(INPUT_IMAGE, cv::IMREAD_COLOR);
        cv::Mat processedImage = preProcessImage(image);

        transfer_interpreter_->AllocateTensors();

        int contentImageIndex = fromNameToIndex("content_image", true, false);
        int styleInputIndex = fromNameToIndex("mobilenet_conv/Conv/BiasAdd", true, false);

        std::cout << "Content index: " << contentImageIndex << std::endl;
        std::cout << "Style index: " << styleInputIndex << std::endl;
        printVector(transfer_interpreter_ -> inputs());

        auto contentBuffer = transfer_interpreter_->typed_tensor<float>(contentImageIndex);
        auto styleBuffer = transfer_interpreter_->typed_tensor<float>(styleInputIndex);

        TfLiteIntArray* styleDims = transfer_interpreter_->tensor(styleInputIndex)->dims;
        TfLiteIntArray* contentDims = transfer_interpreter_->tensor(contentImageIndex)->dims;

        unsigned int styleSize = sizeof(float);
        unsigned int contentSize = sizeof(float);

        for(int i = 1; i < styleDims->size; ++i) {
            styleSize = styleSize * styleDims->data[i];
        }
        for(int j = 1; j < contentDims->size; ++j) {
            contentSize = contentSize * contentDims->data[j];
        }
        memcpy(contentBuffer, processedImage.data, contentSize);
        memcpy(styleBuffer, styleVec.data(), styleSize);

        if(transfer_interpreter_->Invoke() == kTfLiteOk) {
            auto outputIndex = fromNameToIndex("transformer/expand/conv3/conv/Sigmoid", false, false);
            TfLiteIntArray* dims = transfer_interpreter_->tensor(outputIndex)->dims;
            int outputSize = 1;
            for(int i = 1; i < dims->size; ++i) {
                outputSize = outputSize * dims->data[i];
            }

            int width = dims->data[1];
            int height = dims->data[2];

            cv::Size outputImageSize = cv::Size(width, height);
            // Get the data out of the outputBuffer
            const float * outputBuffer = transfer_interpreter_->typed_tensor<float>(outputIndex);

            auto tensorMat = cv::Mat(outputImageSize, CV_32FC3, (void *) outputBuffer);
            cv::Mat outputImage;
            const cv::Scalar maxVal = cv::Scalar(255, 255, 255);
            cv::multiply(tensorMat, maxVal, outputImage);
            outputImage.convertTo(outputImage, CV_8UC3);
            cv::cvtColor(outputImage, outputImage, cv::COLOR_BGR2RGB);

            std::string outputString = APP_PATH + "/output.jpg";
            cv::imwrite(outputString, outputImage);

            return outputString;
        } else {
            return "";
        }
    }
    return "";

}


std::vector<float> StyleTransfer::getStyle(int styleVal) {

    std::cout << "Getting style" << std::endl;

    std::string styleImage;

    switch(styleVal) {
        case(0) :
            styleImage = GRAND_CANYON;
            break;
        case(1) :
            styleImage = GRAND_CANYON;
            break;
        case(2) :
            styleImage = GRAND_CANYON;
            break;
        default :
            styleImage = GRAND_CANYON;
            break;
    }

    cv::Mat styleMat = cv::imread(styleImage, cv::IMREAD_COLOR);
    cv::cvtColor(styleMat, styleMat, cv::COLOR_BGR2RGB);

    styleMat.convertTo(styleMat, CV_32FC3, 1.f/255.0f);

    std::string inputName = "style_image";
    auto inputIndex = fromNameToIndex(inputName, true, true);

    style_interpreter_ -> AllocateTensors();

    auto tensorBuffer = style_interpreter_ -> typed_tensor<float>(inputIndex);
    unsigned int tensorSize = styleMat.total() * styleMat.elemSize();
    memcpy((void *) tensorBuffer, (void *) styleMat.data, tensorSize);

    if(style_interpreter_->Invoke() != kTfLiteOk) {
        
        // Return the empty vector
        std::vector<float> emptyVec;
        std::cout << "TFLite error!!!!!" << std::endl;
        return emptyVec;
    } 
    else {

        auto outputIndex = fromNameToIndex("mobilenet_conv/Conv/BiasAdd", false, true);

        // First element in the output shape is the batch size.
        TfLiteIntArray* dims = style_interpreter_->tensor(outputIndex)->dims;
        int outputSize = 1;
        for(int i = 1; i < dims->size; ++i) {
            outputSize = outputSize * dims->data[i];
        }
        size_t outputByteSize = outputSize * sizeof(float);

        std::vector<float> outputFloat;
        outputFloat.resize(outputSize);

        // Get the data out of the outputBuffer (I don't think we actually need to do a memcpy here)
        const float * outputBuffer = style_interpreter_->typed_tensor<float>(outputIndex);
        memcpy(outputFloat.data(), outputBuffer, outputByteSize);

        return outputFloat;
    }
}



int StyleTransfer::fromNameToIndex(std::string stdName, bool isInput, bool isStylePredict) const {
    
    ::tflite::Interpreter * interpreter;

    if(isStylePredict)
        interpreter = style_interpreter_.get();
    else
        interpreter = transfer_interpreter_.get();

    int len = isInput ? interpreter->inputs().size() : interpreter->outputs().size();

    for (int tIndex = 0; tIndex < len; ++tIndex){
        std::string tName = std::string(isInput ? interpreter->GetInputName(tIndex)
                                                : interpreter->GetOutputName(tIndex));
        if (tName == stdName){
            return isInput ? interpreter->inputs()[tIndex]
                           : interpreter->outputs()[tIndex];
        }
    }
    return -1;
}


StyleTransfer::~StyleTransfer() {

    // NEW: Clean up.
    TfLiteGpuDelegateDelete(delegate);
}

cv::Mat StyleTransfer::preProcessImage(cv::Mat input) {

    std::string firstImageStr = APP_PATH + "/testcppwhat.jpg";
    cv::imwrite(firstImageStr, input);

    cv::Mat resizedImage;
    cv::Size imageSize(384, 384);
    cv::resize(input, resizedImage, imageSize);

    std::string outputString = APP_PATH + "/testcpp.jpg";
    cv::imwrite(outputString, resizedImage);

    resizedImage.convertTo(resizedImage, CV_32F, 1.f/255);

    return resizedImage;
}