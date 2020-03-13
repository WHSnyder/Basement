#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>

#include <glm.hpp>
#include <ext.hpp>


/*
Wrapper class for OpenGL/GLSL shaders.  Different matrix and texture types have 
standardized names in GLSL so other classes dont have to reference them by string
name. 
*/


class Shader {

	private:

		GLuint data_texture, image_texture, shadow_texture;
		GLuint proj_loc, view_loc, model_loc, col_loc;

	public:

		GLuint progID;

		Shader(string shaderpath);

		void setModel(float *model);
		void setView(float *view);
		void setProj(float *proj);

		void setFloat(string name, float f);
		void setVec3(string name, glm::vec3 v);
		void setMat4(string name, glm::mat4 m);
		void setColor(glm::vec3 v);

		int setDataTexture(GLuint tID, GLuint slot);
		int setImageTexture(GLuint tID, int cubemap, int unit);
		int setShadowTexture(GLuint tID);

		void printUniforms();
		void getUniforms(vector<string>& names, vector<GLenum>& types);

		~Shader(){
			glDeleteProgram(progID);
		}
};