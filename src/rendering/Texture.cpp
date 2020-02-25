#include "rendering/Texture.h"



GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__) 




GLuint bindTexture(int cols, int rows, float *data, int color){

	GLuint tex;

	glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glCheckError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glCheckError();

    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                color ? GL_RGB : GL_RED,            // Internal colour format to convert to
                cols,
                rows,
                0,                 // Border width in pixels (can either be 1 or 0)
                color ? GL_RGB : GL_RED, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                GL_FLOAT,  // Image data type
                (void *) data);        // The actual image data itself

    glGenerateMipmap(GL_TEXTURE_2D);
    glCheckError();

    return tex;
}


Texture::Texture(float *_data, int width, int height, int color){
	rows = height, cols = width;
	texID = bindTexture(cols, rows, _data, color);
	data = _data;
}