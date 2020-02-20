#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <chrono>

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
	   
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &contents, NULL);
	glCompileShader(shader);
	glCheckError();

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(!success){
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	  
	return shader;
}


int main(int argc, char **argv){
	
	GLFWwindow* window;

	string path = "/Users/will/projects/cpprtx/meshes/cube.obj";
	Mesh *cube = new Mesh(path);	

	if(!glfwInit()){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(480, 480, "Tutorial 02 - Red triangle", NULL, NULL);
	if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetKeyCallback(window, key_callback);

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	//glCheckError();

   	cube -> bindBuffers();

    string vshader = read_shader("src/rendering/shaders/BasicVert.hlsl");
    string fshader = read_shader("src/rendering/shaders/BasicFrag.hlsl");

    unsigned int vertex = compile_shader(GL_VERTEX_SHADER, vshader);
    unsigned int fragment = compile_shader(GL_FRAGMENT_SHADER, fshader);
    glCheckError();

    unsigned int ID = glCreateProgram();
    int success;

    char infoLog[512];

    ID = glCreateProgram();
    glCheckError();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glCheckError();

	glLinkProgram(ID);
	glCheckError();

	glUseProgram(ID);
	glCheckError();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glCheckError();


    auto t_start = std::chrono::high_resolution_clock::now();
	auto t_now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

	mat4 proj = perspective(glm::radians(45.0f), 1.0f, 1.0f, 90.0f);
	mat4 trans = mat4(1.0);
    //trans = rotate(trans, time * radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
    
    GLint projloc = glGetUniformLocation(ID, "p");
    GLint rotloc = glGetUniformLocation(ID, "m");

	glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(rotloc, 1, GL_FALSE, glm::value_ptr(trans));

	glCheckError();


	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearDepth(0.0);
		glCheckError();

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		t_start = t_now;

		trans = rotate(trans, time * glm::radians(20.0f), vec3(0.0f,0.0f,1.0f));
		glUniformMatrix4fv(rotloc, 1, GL_FALSE, glm::value_ptr(trans));
		glCheckError();

		cube -> draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	cube -> deleteBuffers();
	glDeleteProgram(ID);
	glfwTerminate();

	return 0;
}