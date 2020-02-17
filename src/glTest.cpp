#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include <mesh/Mesh.h>

#include <utils/ShaderUtils.h>


using namespace std;


static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


int compile_shader(GLenum shaderType, string shaderCode){
	
	unsigned int shader;
	int success;
	char infoLog[512];
	const char *contents = shaderCode.c_str();
	   
	// shader Shader
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &contents, NULL);
	glCompileShader(shader);

	// print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(!success){
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	  
	return shader;
}








int main(int argc, char **argv){

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(480, 480, "Simple example", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);


    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);


    glewExperimental = GL_TRUE;
    glewInit();

	Mesh *cube = new Mesh("./meshes/cube.obj");	
	cube -> bindBuffers();

    string vshader = read_shader("src/rendering/shaders/BasicVert.hlsl");
    cout << "VERTEX SHADER: \n" << vshader << endl;

    string fshader = (const char *) read_shader("src/rendering/shaders/BasicFrag.hlsl").c_str();
    cout << "FRAG SHADER: \n" << fshader << endl;

    unsigned int vertex = compile_shader(GL_VERTEX_SHADER, vshader);
    unsigned int fragment = compile_shader(GL_FRAGMENT_SHADER, fshader);

    unsigned int ID = glCreateProgram();
    int success;

    char infoLog[512];

    ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if(!success){
	    glGetProgramInfoLog(ID, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

    
    while (!glfwWindowShouldClose(window)){

    	glClearColor(1.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}