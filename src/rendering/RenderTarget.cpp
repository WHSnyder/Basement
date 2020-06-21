#include "rendering/RenderTarget.h"

#include <iostream>

extern GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#define COUT(x) std::cout << x << std::endl;


using namespace std;


void RenderTarget::set(int clear){

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	if (clear == 1)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (clear == 2)
		glClear(GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, cols, rows);
}


void bindShadowBuffer(GLuint& framebufferID, GLuint& texID, int rows, int cols){

	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	glGenTextures(1, &texID); 
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, cols, rows, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texID, 0);
	
	glDrawBuffer(GL_NONE); 

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		cout << "framebuffer incomplete" << endl;
		glCheckError();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void bindTextureFramebuffer(GLuint& framebufferID, GLuint& depthBufferID, GLuint& texID, GLenum *outBuffers, int rows, int cols){

	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, cols, rows);
	glCheckError();

	COUT("SUBIMAGE INBOUND")

	unsigned char *dummy = new unsigned char[cols * rows * 3]();

	for (int h = 0; h < 200 * 200; h++)
		dummy[h] = 150; //Visual sanity check 

	//Dummy texture data
	/*glTexSubImage2D(GL_TEXTURE_2D,     
                0,                 
                0,
                0,          
                cols,
                rows,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                dummy); // <- Causes seg fault?  A lot of docs use this...
	*/
	delete[] dummy;

	//Depth buffer
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, cols, rows);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);

	outBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, outBuffers); 

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		COUT("Framebuffer incomplete")
		glCheckError();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



RenderTarget::RenderTarget(int rows_, int cols_, int type){

	rows = rows_;
	cols = cols_;

	outBuffers = new GLenum[3]();

	COUT("Starting render texture bind")

	if (type == 0) {
		bindTextureFramebuffer(framebufferID, depthBufferID, texID, outBuffers, rows, cols);
		numOutBuffers = 1;
	} else {
		bindShadowBuffer(framebufferID, texID, rows, cols);
		numOutBuffers = 0;
	}
}	


#ifdef PYBIND
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(RenderTarget, m) {
    py::class_<RenderTarget>(m, "RenderTarget")
        .def(py::init<int, int, int>())
        .def("set", &RenderTarget::set);
}
#endif