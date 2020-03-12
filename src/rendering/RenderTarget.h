#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>

class Node;

class RenderTarget {
	
	private:

		GLuint framebufferID = 0, depthBufferID = 0, colorAttachmentID = 0, texID = 0;
		GLenum *outBuffers;
		int numOutBuffers, rows, cols;

	public:

		RenderTarget(int rows_, int cols_, int shadow);
		RenderTarget(Node node);
		
		void set();

		inline GLuint getTexture(){
			return texID;
		}

		~RenderTarget(){
			glDeleteFramebuffers(1, &framebufferID);
			delete outBuffers;
		}
};