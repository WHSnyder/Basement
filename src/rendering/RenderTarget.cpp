#include "rendering/RenderTarget.h"
#include "rendering/Texture.h"


extern GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 


using namespace std;


Texture RenderTarget::getTexture(){

	Texture tex();
	tex.texID = texID;
	tex.cols = cols;
	tex.rows = rows;

	return tex;
}


void RenderTarget::set(){
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glViewport(0,0,cols,rows);
}


void bindShadowbuffer(Gluint& framebufferID, GLuint& texID, int rows, int cols){

	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, cols, rows, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texID, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		cout << "framebuffer incomplete" << endl;
		glCheckError();
	}
}


void bindTextureFramebuffer(Gluint& framebufferID, GLuint& depthBufferID, GLuint& texID, GLenum& *outBuffers, int rows, int cols){

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
	GLuint depthBufferID;
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, cols, rows);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);

	outBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, outBuffers); 

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		cout << "Framebuffer incomplete" << endl;
		glCheckError();
	}
}





RenderTarget::RenderTarget(int rows_, int cols_, int shadow){

	rows = rows_;
	cols = cols_;

	if (shadow){

		bindShadowbuffer(framebufferID, texID, rows, cols);
		numOutBuffers = 0;
	}
	else {

		bindTextureFramebuffer(framebufferID, depthBufferID, texID, outBuffers, rows, cols);
		numOutBuffers = 1;
	}	
}	