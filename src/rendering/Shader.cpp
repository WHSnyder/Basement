#include "rendering/Shader.h"
#include "utils/ShaderUtils.h"
#include "rendering/Texture.h"

#include <iostream>


using namespace std;

extern string basepath;

extern GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
#define COUT(x) std::cout << x << std::endl;

GLenum texEnums[] = {GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_3D};

/*
void Shader::Dispatch(int w, int l, int d, int barrier){
	glUseProgram(progID);
	glDispatchCompute(w,l,d);
}
*/


int compile_shader(GLenum shaderType, string shaderCode){
	
	GLuint shader;
	int success;
	char infoLog[512];
	const char *contents = shaderCode.c_str();
	   
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &contents, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(!success){
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "Shader type = " << shaderType << std::endl;
	};

	glCheckError();
	  
	return shader;
}


GLuint build_compute_program(string shader_path){

	std::cout << "Building compute program " << shader_path << std::endl;

	string cshader = string(shader_path);
	cshader.append("_c.glsl");
    string cshader_txt = read_shader(cshader);

    GLuint c_id = compile_shader(GL_COMPUTE_SHADER, cshader_txt);
    GLuint prog_id = glCreateProgram();
    glAttachShader(prog_id, c_id);
	glLinkProgram(prog_id);

	glCheckError();

	return prog_id;
}


GLuint build_program(string shader_path){

	COUT(string("Building raster program ") + shader_path)
 
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
	glLinkProgram(prog_id);

	glCheckError();

	return prog_id;
}


Shader::Shader(string shader_path, int buildCompute){

	if (buildCompute)
		progID = build_compute_program(shader_path);
	else 
		progID = build_program(shader_path);

	glUseProgram(progID); 

	//For engine special vars, before I knew about glGetActiveUniform
	data_texture = glGetUniformLocation(progID, "dataTex");
	image_texture = glGetUniformLocation(progID, "imageTex");
	proj_loc = glGetUniformLocation(progID, "p");
	view_loc = glGetUniformLocation(progID, "v");
	model_loc = glGetUniformLocation(progID, "m");
	col_loc = glGetUniformLocation(progID, "color");
	shadow_texture = glGetUniformLocation(progID, "shadowTex");
}


int Shader::set2DTexture(GLuint texID, string texName, int unit){

	GLuint tempID = glGetUniformLocation(progID, texName.c_str());

	glUseProgram(progID);
	glUniform1i(tempID, unit);
    glActiveTexture(GL_TEXTURE0 + unit); //The nuance of these calls is still hazy from memory...
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glCheckError();

    return 0;
}


int Shader::set3DTexture(GLuint texID, string texName, int unit){

	GLuint tempID = glGetUniformLocation(progID, texName.c_str());

	glUseProgram(progID);
	glUniform1i(tempID, unit);
    glActiveTexture(GL_TEXTURE0 + unit); //The nuance of these calls is still hazy from memory...
	glBindTexture(GL_TEXTURE_3D, texID);
	
	glCheckError();

    return 0;
}


int Shader::setImageTexture(GLuint tID, int cubemap, int unit){

	glUseProgram(progID); 

	glUniform1i(image_texture, unit);

    glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, tID);
	glCheckError();

    return 0;
}


int Shader::setDataTexture(GLuint tID, GLuint slot){

	glUseProgram(progID); 

	glUniform1i(data_texture, slot);

    glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tID);

    return 0;
}


int Shader::setShadowTexture(GLuint tID){

	glUseProgram(progID); 

	glUniform1i(shadow_texture, 10);

    glActiveTexture(GL_TEXTURE0 + 10);
	glBindTexture(GL_TEXTURE_2D, tID);

    glCheckError();

    return 0;
}


void Shader::setModel(float *model){
	glUseProgram(progID);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
}


void Shader::setView(float *view){
	glUseProgram(progID);
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);
}


void Shader::setProj(float *proj){
	glUseProgram(progID);
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj);
}


void Shader::setInt(string name, int i){

	GLuint tempID;

	glUseProgram(progID); 
	tempID = glGetUniformLocation(progID, name.c_str());
	glUniform1i(tempID, i);
}


void Shader::setFloat(string name, float f){
	
	GLuint tempID;

	glUseProgram(progID); 
	tempID = glGetUniformLocation(progID, name.c_str());
	glUniform1f(tempID, f);
}

void Shader::setVec2(std::string name, const glm::vec2& v){

	GLuint tempID;

	glUseProgram(progID); 
	tempID = glGetUniformLocation(progID, name.c_str());
	glUniform2fv(tempID, 1, glm::value_ptr(v));	
}


void Shader::setVec3(string name, const glm::vec3& v){

	GLuint tempID;

	glUseProgram(progID); 
	tempID = glGetUniformLocation(progID, name.c_str());
	glUniform3fv(tempID, 1, glm::value_ptr(v));	
}

void Shader::setMat4(string name, const glm::mat4& m){

	GLuint tempID;

	glUseProgram(progID); 
	tempID = glGetUniformLocation(progID, name.c_str());
	glUniformMatrix4fv(tempID, 1, GL_FALSE, glm::value_ptr(m));	
}

void Shader::setColor(const glm::vec3& v){

	glUseProgram(progID); 
	glUniform3fv(col_loc, 1, glm::value_ptr(v));	
}

void Shader::getUniforms(vector<string>& names, vector<GLenum>& types, int filter){

	GLint i, count, size;
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 16; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length;

	string model_str = string("m"), view_str = string("v"), proj_str = string("p");

	glUseProgram(progID);
	glGetProgramiv(progID, GL_ACTIVE_UNIFORMS, &count);
	
	for (i = 0; i < count; i++){
	    
	    glGetActiveUniform(progID, (GLuint) i, bufSize, &length, &size, &type, name);

	    if (filter && (model_str.compare(name) || 
	    	view_str.compare(name) || 
	    	proj_str.compare(name))){

	    	continue;
	    }

	   	names.push_back(string(name));
	    types.push_back(type);
	}
}

void Shader::printUniforms(){
	
	vector<GLenum> types;
	vector<string> names;

	getUniforms(names, types, 0);
	
	printf("Active Uniforms: %d\n", names.size());

	for (int i = 0; i < names.size(); i++){
	    //printf("Uniform #%d Type: %u Name: %s\n", i, types[i], names[i]);
	}
}


#ifdef PYBIND
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(Shader, m) {
    py::class_<Shader>(m, "Shader")
        .def(py::init<std::string &>())
        .def("printUniforms", &Shader::printUniforms);
}
#endif