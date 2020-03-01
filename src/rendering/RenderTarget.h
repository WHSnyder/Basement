#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

class Texture;

class RenderTarget {

	public:

		GLuint framebufferID, depthBufferID, colorAttachmentID, texID;
		GLenum outBuffers[] = GLenum[3];
		int numOutBuffers, rows, cols;

		RenderTarget(int rows_, int cols_, int shadow);
		void set();

		Texture getTexture();
}