#ifdef PYBIND
#include <pybind11/pybind11.h>
#endif

#include "rendering/Texture.h"

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp> 

using namespace std;

extern string basepath;


GLenum glCheckError_(const char *file, int line){
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        cout << error << " | " << file << " (" << line << ")" << endl;
    }
    return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__) 
#define COUT(x) std::cout << x << std::endl;


vector<string> face_tags({string("_rt"), string("_lf"), string("_up"), string("_dn"), string("_bk"), string("_ft")});


Texture::~Texture(){

    std::cout << "Deleting texture " << texID;
    if (path.length() > 0)
        std::cout << " loaded from " << path.c_str();
    std::cout << std::endl;

    if (data != nullptr)
        delete data; 
    glDeleteTextures(1,&texID);
}


GLuint load3DTexture(int w, int h, int d){

    GLuint texID;
    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, texID);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexStorage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, w, h, d);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, w, h, d, 0, GL_RGBA, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_3D);
    glBindImageTexture(0, texID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glCheckError();

    return texID;
}

GLuint loadCubemap(string basepath, string extension){

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (int i = 0; i < face_tags.size(); i++){
        
        cv::Mat img = cv::Mat(imread(basepath + face_tags[i] + extension, cv::IMREAD_COLOR));
        cv::resize(img, img, cv::Size(1024,1024), 0, 0, cv::INTER_LINEAR);

        if (img.data)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);

        else cout << "Cubemap tex failed to load at path: " << basepath << endl;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

GLuint bindTexture(int color, int rows, int cols, void *data, int mips=4){

	GLuint tex;

	glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexStorage2D(GL_TEXTURE_2D, mips, GL_RGBA32F, cols, rows);

    if (data != nullptr){
        glTexSubImage2D(GL_TEXTURE_2D,     // tex type
                    0,                 // 0 mipmap level = top level
                    0,
                    0,          
                    cols,
                    rows,
                    color ? GL_RGB : GL_RED, // Single channel for perlin map, rgb for opencv
                    color ? GL_UNSIGNED_BYTE : GL_FLOAT,  //If color, were using opencv image, else, perlin map
                    data); 
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    return tex;
}

Texture::Texture(int w, int h, int d){
    texID = load3DTexture(w,h,d);
    data = nullptr;
}

Texture::Texture(int width, int height){
    
    rows = height, cols = width;
    texID = bindTexture(1, rows, cols, nullptr, 1);
}

Texture::Texture(float *_data, int width, int height, int color){
    
    rows = height, cols = width;
    data = (void *) _data;
	texID = bindTexture(0, rows, cols, data);
}

Texture::Texture(string filepath, int cubemap, std::string extension){

    filepath.insert(0, basepath);

    path = filepath;

    if (!cubemap){
        
        cv::Mat img = imread(filepath, cv::IMREAD_COLOR);
        cv::resize(img, img, cv::Size(600,600), 0, 0, cv::INTER_LINEAR);

        rows = 600, cols = rows;
        data = malloc(3 * rows * cols);

        memcpy(data, (void *) img.data, 3 * rows * cols);

        texID = bindTexture(1, rows, cols, data);
        glCheckError();
    }

    else {

        texID = loadCubemap(filepath, extension);   
        rows = 512, cols = 512;     
    }
}

#ifdef PYBIND
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(Texture, m) {
    py::class_<Texture>(m, "Texture")
        .def(py::init<float *, int, int, int>())
        .def(py::init<std::string &, int, std::string>());
}
#endif