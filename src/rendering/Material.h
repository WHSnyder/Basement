#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include <utils/ShaderUtils.h>



class Material {

	private:

		GLuint progID;
		Gluint model;
		GLuint texture;
		float *tex_data = nullptr;

	public:

		static void *view_ptr, frustum_ptr;
}