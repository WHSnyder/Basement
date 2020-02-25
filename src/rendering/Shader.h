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
		GLuint data_texture, image_texture;
		GLuint proj_loc, view_loc, model_loc;

	public:

		Shader(string shaderpath);

		void setMats(float *model, float *view, float *proj);
		void printUniforms();
		int setDataTexture(Texture *tex);

		~Shader(){
			glDeleteProgram(progID);
		}
};