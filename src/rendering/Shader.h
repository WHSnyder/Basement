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

		GLuint data_texture, image_texture;
		GLuint proj_loc, view_loc, model_loc;

	public:

		GLuint progID;

		Shader(string shaderpath);

		void setMats(float *model, float *view, float *proj);
		void printUniforms();
		int setDataTexture(Texture *tex);
		int setImageTexture(Texture *tex);

		~Shader(){
			glDeleteProgram(progID);
		}
};