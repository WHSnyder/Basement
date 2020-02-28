





class RenderTarget {


	public:

		GLuint framebufferID, depthBufferID, colorAttachmentID, texID;
		GLenum outBuffers[] = GLenum[3];
		int numOutBuffers;

		RenderTarget(int rows, int cols);

}
