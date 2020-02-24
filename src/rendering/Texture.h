#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>




class Texture {

	public:

		float *data = nullptr;
		GLuint texID;
		int rows,cols;

		Texture(float *data, int width, int height, int color);

		~Texture(){
			delete data;
			glDeleteTextures(1,&texID);
		}
}