#include "rendering/Shader.h"
#include "utils/ShaderUtils.h"

extern GLenum glCheckError_(const char *file, int line);

//extern GLenum glCheckError();//const char *file, int line);
/*{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}*/

#define glCheckError() glCheckError_(__FILE__, __LINE__) 



int compile_shader(GLenum shaderType, string shaderCode){
	
	unsigned int shader;
	int success;
	char infoLog[512];
	const char *contents = shaderCode.c_str();
	   
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &contents, NULL);
	glCompileShader(shader);
	//glCheckError();

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(!success){
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "Shader type = " << shaderType << std::endl;
	};
	  
	return shader;
}


GLuint build_program(string shader_path){

	GLuint v_id,f_id, prog_id;

	string vshader = string(shader_path);
	string fshader = string(shader_path);

	vshader.append("_v.glsl");
	fshader.append("_f.glsl");

	string vshader_txt = read_shader(vshader);
    string fshader_txt = read_shader(fshader);

    v_id = compile_shader(GL_VERTEX_SHADER, vshader_txt);
    f_id = compile_shader(GL_FRAGMENT_SHADER, fshader_txt);

    glCheckError();

    prog_id = glCreateProgram();

    glAttachShader(prog_id, v_id);
	glAttachShader(prog_id, f_id);
	glCheckError();

	glLinkProgram(prog_id);

	return prog_id;
}


Shader::Shader(string shader_path){

	progID = build_program(shader_path);

	glUseProgram(progID); 

	data_texture = glGetUniformLocation(progID, "tex");
	proj_loc = glGetUniformLocation(progID, "p");
	view_loc = glGetUniformLocation(progID, "v");
	model_loc = glGetUniformLocation(progID, "m");
}


int Shader::setDataTexture(Texture *tex){

	glUseProgram(progID); 

	glUniform1i(data_texture, 0);

    glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, tex->texID);

	glCheckError();

	GLint n_dim = glGetUniformLocation(progID, "dim");
    glUniform1f(n_dim,tex->cols);

    glCheckError();
    return 0;
}

/*
Shader::Shader(Texture *tex, string shader_path){

	progID = build_program(shader_path);

	glUseProgram(progID); 

	GLint n_texloc = glGetUniformLocation(progID, "tex");

    glUniform1i(texture, 0);

    glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);

    GLint n_dim = glGetUniformLocation(progID, "dim");
    glUniform1f(n_dim,tex->cols);
}*/


void Shader::setMats(float *model, float *view, float *proj){

	glUseProgram(progID);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj);
}


void Shader::printUniforms(){
	
	GLint i, count, size;
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 16; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length;

	glUseProgram(progID);
	glGetProgramiv(progID, GL_ACTIVE_UNIFORMS, &count);
	
	printf("Active Uniforms: %d\n", count);

	for (i = 0; i < count; i++){
	    
	    glGetActiveUniform(progID, (GLuint) i, bufSize, &length, &size, &type, name);
	    printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
	}
}
