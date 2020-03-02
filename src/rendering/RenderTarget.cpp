#include "rendering/RenderTarget.h"

#include <iostream>

extern GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

using namespace std;


void RenderTarget::set(){

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCheckError();
	glViewport(0, 0, cols, rows);
}


void bindShadowbuffer(GLuint& framebufferID, GLuint& texID, int rows, int cols){

	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glCheckError();

	//cout << "Shadow buffer ID: " << framebufferID << endl;

	//Active texture unit must be set if new framebuffer is created after binding other textures!
	//glActiveTexture(GL_TEXTURE0 + 4);  

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &texID); //bug here and below....
	glBindTexture(GL_TEXTURE_2D, texID);

	cout << "Bound new texture at " << texID << endl;

	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, cols, rows, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texID, 0);
	

	glDrawBuffer(GL_NONE); 
	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		cout << "framebuffer incomplete" << endl;
		glCheckError();
	}
	//glGenerateMipmap(GL_TEXTURE_2D);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void bindTextureFramebuffer(GLuint& framebufferID, GLuint& depthBufferID, GLuint& texID, GLenum *outBuffers, int rows, int cols){

	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	
	//Now we need to create the texture which will contain the RGB output of our shader. This code is very classic :
	// The texture we're going to render to = texID
	glGenTextures(1, &texID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texID);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, cols, rows);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);

	outBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, outBuffers); 

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		cout << "Framebuffer incomplete" << endl;
		glCheckError();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


RenderTarget::RenderTarget(int rows_, int cols_, int shadow){

	rows = rows_;
	cols = cols_;

	outBuffers = new GLenum[3]();

	if (shadow){
		bindShadowbuffer(framebufferID, texID, rows, cols);
		numOutBuffers = 0;
	}
	else {
		bindTextureFramebuffer(framebufferID, depthBufferID, texID, outBuffers, rows, cols);
		numOutBuffers = 1;
	}	
}	