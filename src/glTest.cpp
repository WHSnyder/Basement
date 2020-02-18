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

	string path = "/Users/will/projects/cpprtx/meshes/cube.obj";
	Mesh *cube = new Mesh(path);	

	if( !glfwInit() )
	{
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
	window = glfwCreateWindow( 480, 480, "Tutorial 02 - Red triangle", NULL, NULL);
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
	glDepthFunc(GL_GEQUAL); 

   	cube -> bindBuffers();

    string vshader = read_shader("src/rendering/shaders/BasicVert.hlsl");
    string fshader = read_shader("src/rendering/shaders/BasicFrag.hlsl");

    unsigned int vertex = compile_shader(GL_VERTEX_SHADER, vshader);
    unsigned int fragment = compile_shader(GL_FRAGMENT_SHADER, fshader);

    unsigned int ID = glCreateProgram();
    int success;

    char infoLog[512];

    ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glUseProgram(ID);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	do{

		glClear( GL_COLOR_BUFFER_BIT );

		cube -> draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	cube -> deleteBuffers();
	glDeleteProgram(ID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
    

