/*
#ifdef __APPLE__
    #include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
    #include <OpenGL/gl3ext.h>      /// ditto
#else 
    #include <GL/glew.h>
#endif
*/
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


int compile_shader(GLenum shaderType, char *shaderCode){
	
	unsigned int shader;
	int success;
	char infoLog[512];
	   
	// shader Shader
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &shaderCode, NULL);
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

	

	//GLint nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;


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

	//#ifndef __APPLE__
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        std::cerr << "GLEW failed to initialize!" << std::endl;
    }
	//#endif

	Mesh *cube = new Mesh("./meshes/cube.obj");	

	cout << "Read mesh" << endl;

	cube -> bindBuffers();

	cout << "Bound buffers" << endl;


    char *vshader = (char *) read_shader("src/rendering/shaders/BasicVert.hlsl").c_str();
    char *fshader = (char *) read_shader("src/rendering/shaders/BasicFrag.hlsl").c_str();

    cout << "Read shaders" << endl;

    cout << glGetString(GL_VERSION) << endl;

    compile_shader(GL_VERTEX_SHADER, vshader);
    compile_shader(GL_FRAGMENT_SHADER, fshader);

    cout << "Compiled shaders" << endl;


    
    while (!glfwWindowShouldClose(window)){

    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT);       
    	glMatrixMode(GL_PROJECTION);
        
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);


        
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}