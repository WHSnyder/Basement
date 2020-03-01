#include "rendering/Texture.h"

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp> 

using namespace std;


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


vector<string> face_tags({string("_rt"), string("_lf"), string("_up"), string("_dn"), string("_bk"), string("_ft")});


GLuint loadCubemap(string basepath){

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    cv::Mat *img;

    for (int i = 0; i < face_tags.size(); i++){
        
        img = new cv::Mat(imread(basepath + face_tags[i], cv::IMREAD_COLOR));
        cv::resize(*img, *img, cv::Size(512,512), 0, 0, cv::INTER_LINEAR);

        if (img -> data){

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, img -> data);
        }

        else cout << "Cubemap tex failed to load at path: " << basepath << endl;

        delete img;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


GLuint bindTexture(int color, int rows, int cols, void *data){

	GLuint tex;

	glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glCheckError();

    //interp method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    //clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glCheckError();

    glTexImage2D(GL_TEXTURE_2D,     // tex type
                0,                 // 0 mipmap level = top level
                color ? GL_RGB : GL_RED,            
                cols,
                rows,
                0,                 // Border 0 for now, might change with atlessing + mipmapping
                color ? GL_RGB : GL_RED, // Single channel for perlin map, rgb for opencv
                color ? GL_UNSIGNED_BYTE : GL_FLOAT,  //If color, were using opencv image, else, perlin map
                data); 

    glGenerateMipmap(GL_TEXTURE_2D);
    glCheckError();

    return tex;
}


Texture::Texture(float *_data, int width, int height, int color){
    
    rows = height, cols = width;
    data = (void *) _data;
	texID = bindTexture(0, rows, cols, data);
}


Texture::Texture(string filepath, int cubemap){

    if (!cubemap){
        
        cv::Mat img = imread(filepath, cv::IMREAD_COLOR);
        cv::resize(img, img, cv::Size(512,512), 0, 0, cv::INTER_LINEAR);

        rows = 512, cols = 512;

        data = malloc(3 * rows * cols);

        memcpy(data, (void *) img.data, 3 * rows * cols);

        texID = bindTexture(1, rows, cols, data);
    }

    else {

        texID = loadCubemap(filepath);   
        rows = 512, cols = 512;     
    }
}