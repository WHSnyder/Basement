#include "rendering/Shader.h"
#include "utils/ShaderUtils.h"


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

}


Shader::Shader(float *data, int rows, int cols, string shader_path){

	progID = build_program(shader_path);
	texture = bindTexture(cols,rows,data);

	glUseProgram(progID);

	GLint n_texloc = glGetUniformLocation(progID, "tex");

    glUniform1i(texture, 0);

    glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);

    GLint n_dim = glGetUniformLocation(n_ID, "dim");
    glUniform1f(n_dim,cols);
}