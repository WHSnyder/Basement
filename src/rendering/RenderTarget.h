#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>


class RenderTarget {
	
	private:

		GLuint framebufferID, depthBufferID, colorAttachmentID, texID;
		GLenum *outBuffers;
		int numOutBuffers, rows, cols;

	public:

		RenderTarget(int rows_, int cols_, int shadow);
		
		void set();
		GLuint getTexture();

		~RenderTarget(){
			glDeleteFramebuffers(1, &framebufferID);
			delete outBuffers;
		}
};