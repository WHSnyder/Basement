#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include "utils/ShaderUtils.h"
#include "rendering/Texture.h"



class Shader {

	private:

		GLuint progID;
		GLuint texture;
		GLuint proj_loc, view_loc, model_loc;

	public:

		static void *view_ptr, frustum_ptr;

		Shader(Texture *tex, string shaderpath);
		Shader(string shaderpath);

		void setMats(float *model, float *view, float *proj);
		void printUniforms();

		~Shader(){
			delete tex_data;
			glDeleteProgram(ID);
			glDeleteTextures(1,&texture);
		}


}