#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include <mesh/Mesh.h>
#include <phys/Physics.h>
#include <gtx/transform.hpp>
#include <perlin/PerlinNoise.hpp>

#include "rendering/Texture.h"
#include "rendering/Shader.h"

using namespace std;

void coutMat(float *mat){

	cout << "{" << mat[0] << ", " << mat[1] << ", " << mat[2] << ", " << mat[3] << "}" << endl;
	cout << " " << mat[4] << ", " << mat[5] << ", " << mat[6] << ", " << mat[7] << "," << endl;
	cout << " " << mat[8] << ", " << mat[9] << ", " << mat[10] << ", " << mat[11] << "," << endl;
	cout << " " << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << "}" << endl;
}

float *generate_terrain(int rows, int cols){

	cout << "Generating terrain" << endl;

	std::uint32_t seed = 6782;

	const siv::PerlinNoise perlin(seed);
	const siv::PerlinNoise perlin2(5892);
	float *result = (float *) calloc(cols * rows, sizeof(float));
	double freq = 16.0;
	double fx = cols/freq, fy = rows/freq;

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){

			float per = (float) perlin.accumulatedOctaveNoise2D_0_1(j/fx, i/fy, 16);
			float per2 = (float) perlin.accumulatedOctaveNoise2D_0_1(j/fx, i/fy, 5);
			result[i * cols + j] = .5 * (per + per2);
		}
	}

	return result;
}

static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

Mesh *gen_plane(){

	vec3 verts[] = {vec3(-1,1,1), vec3(1,1,1), vec3(-1,-1,1), vec3(1,-1,1)};
	vec3 norms[] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1)};
	GLuint inds[] = {0,1,2,1,3,2};

	return new Mesh(vector<vec3>(verts,verts+4), vector<vec3>(norms,norms+4), vector<GLuint>(inds,inds+6));
}




int main(int argc, char **argv){
	
	GLFWwindow* window;

	Simu mainSimu;

	string path("/Users/will/projects/cpprtx/meshes/cube.obj");
	Mesh *cube = new Mesh(path);

	string path1("/Users/will/projects/cpprtx/meshes/ball.obj");
	Mesh *sphere = new Mesh(path1);

	string path2("/Users/will/projects/cpprtx/meshes/terrain_plane.obj");
	Mesh *terrain_plane = new Mesh(path2);

	Mesh *plane = gen_plane();

	Shader terrain_shader = Shader("src/rendering/shaders/noise_test");


	int rows = 120, cols = rows;
    float *img_data = generate_terrain(img_data,rows,cols);
    Texture noise_tex = Texture(img_data, rows, cols, 0);

	Shader *perlin = new Material(img_data, rows, cols, "src/rendering/shaders/plane");
	perlin -> printUniforms();

	delte perlin;

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
	window = glfwCreateWindow(480, 480, "Test", NULL, NULL);
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










    auto t_start = std::chrono::high_resolution_clock::now();
	auto t_now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

	mat4 proj = perspective(glm::radians(45.0f), 1.0f, 0.01f, 100.0f);
	mat4 rot = mat4(1.0),testmat;
	mat4 dest = mat4(1.0);
	mat4 trans = transpose(translate(vec3(-1,1.0,-20.0)));
	mat4 trans2 = transpose(translate(vec3(1,1.0,-20.0)));

	mat4 view = lookAt(vec3(0,0,5),vec3(0,0,-100),vec3(0,1.0,0));
    
    /*GLint projloc = glGetUniformLocation(ID, "p");
    GLint rotloc = glGetUniformLocation(ID, "m");
    GLint lookloc = glGetUniformLocation(ID, "v");

	glUniformMatrix4fv(projloc, 1, GL_FALSE, value_ptr(proj));
	glUniformMatrix4fv(lookloc, 1, GL_FALSE, value_ptr((view)));

	glCheckError();
	*/

	float sphereMat[16] = {}, boxMat[16] = {};
	/*
	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		t_start = t_now;

		mainSimu.stepSimu(time);
		mainSimu.getModelMats(sphereMat, boxMat);

		//cout << "=========================" << endl;
		//coutMat((boxMat));
		//cout << "-------------------------" << endl;
		//coutMat(value_ptr(trans));
		//cout << "-------------------------" << endl;
		//testmat = trans * rot;
		//coutMat(value_ptr(testmat));
		//cout << "=========================" << endl;

		glUseProgram(n_ID);
		glUniformMatrix4fv(n_projloc, 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(n_lookloc, 1, GL_FALSE, value_ptr(view));

		terrain_plane -> draw();


		//rot = rotate(rot, time * glm::radians(20.0f), vec3(0.0f,1.0f,0.0f));

		//glUniformMatrix4fv(rotloc, (GLuint) 1, GL_FALSE, boxMat);// value_ptr(transpose(dest)));

		//glCheckError();
		//cube -> draw();

		//memcpy(value_ptr(dest),sphereMat,16 * sizeof(float));

		//glUniformMatrix4fv(rotloc, (GLuint) 1, GL_FALSE, sphereMat);//value_ptr(transpose(dest)));
		//glCheckError();
		
		//sphere -> draw();



		glfwSwapBuffers(window);
		glfwPollEvents();
	} 

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
	*/
	delete cube;
	delete sphere;
	delete terrain_plane;
	delete plane;

	glfwTerminate();

	return 0;
}