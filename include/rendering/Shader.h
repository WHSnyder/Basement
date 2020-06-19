#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


/*
Wrapper class for OpenGL/GLSL shaders.  Different matrix and texture types have 
standardized names in GLSL so other classes dont have to reference them by string
name. 
*/

class Shader {

	private:

		GLuint data_texture, image_texture, shadow_texture;
		GLuint proj_loc, view_loc, model_loc, col_loc;
		int isCompute = 0;

	public:

		GLuint progID;

		Shader(std::string shaderpath, int buildCompute = 0);

		void setModel(float *model);
		void setView(float *view);
		void setProj(float *proj);

		void setInt(std::string name, int i);
		void setFloat(std::string name, float f);
		void setVec2(std::string name, const glm::vec2& v);
		void setVec3(std::string name, const glm::vec3& v);
		void setMat4(std::string name, const glm::mat4& m);
		void setColor(const glm::vec3& v);

		int setDataTexture(GLuint tID, GLuint slot);
		int setImageTexture(GLuint tID, int cubemap, int unit);
		int setShadowTexture(GLuint tID);
		int set3DTexture(GLuint texID, std::string texName, int unit);
		int set2DTexture(GLuint texID, std::string texName, int unit);

		void printUniforms();
		void getUniforms(std::vector<std::string>& names, std::vector<GLenum>& types, int filter=1);

		~Shader(){
			glDeleteProgram(progID);
		}
};