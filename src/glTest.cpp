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
#include "rendering/Shader.h"
#include "utils/controls.hpp"

//#include "Object.h"

using namespace std;

void coutMat(float *mat){

	cout << "{" << mat[0] << ", " << mat[1] << ", " << mat[2] << ", " << mat[3] << "}" << endl;
	cout << " " << mat[4] << ", " << mat[5] << ", " << mat[6] << ", " << mat[7] << "," << endl;
	cout << " " << mat[8] << ", " << mat[9] << ", " << mat[10] << ", " << mat[11] << "," << endl;
	cout << " " << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << "}" << endl;
}

float *generate_terrain(int rows, int cols){

	std::uint32_t seed = 678462;

	const siv::PerlinNoise perlin(seed);
	float *result = (float *) calloc(cols * rows, sizeof(float));
	double freq = 8.0;
	double fx = cols/freq, fy = rows/freq;

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){

			float per = (float) perlin.accumulatedOctaveNoise2D_0_1(j/fx, i/fy, 8);
			result[i * cols + j] = per;
		}
	}

	return result;
}


Mesh gen_plane(){

    vec3 verts[] = {vec3(-1,1,0), vec3(1,1,0), vec3(-1,-1,0), vec3(1,-1,0)};
    vec3 norms[] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1)};
    GLuint inds[] = {0,1,2,1,3,2};

    return Mesh(vector<vec3>(verts,verts+4), vector<vec3>(norms,norms+4), vector<GLuint>(inds,inds+6));
}


static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* window;


int main(int argc, char **argv){
	
	//Simu mainSimu;

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
	window = glfwCreateWindow(600, 600, "Test", NULL, NULL);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  


    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);  



	string path("/Users/will/projects/cpprtx/assets/meshes/cube.obj");
	Mesh cube = Mesh(path);

	string path1("/Users/will/projects/cpprtx/assets/meshes/ball.obj");
	Mesh sphere = Mesh(path1);

	string path2("/Users/will/projects/cpprtx/assets/meshes/terrain_plane.obj");
	Mesh terrain_plane = Mesh(path2);

	Mesh plane = gen_plane();


	int rows = 120, cols = rows;
    float *img_data = generate_terrain(rows,cols);
    Texture noise_tex = Texture(img_data, rows, cols, 0);

	Shader plane_shader = Shader("src/rendering/shaders/plane");
	plane_shader.setDataTexture(&noise_tex);
	
	Shader terrain_shader = Shader("src/rendering/shaders/noise_test");
	terrain_shader.setDataTexture(&noise_tex);
	terrain_shader.setFloat(string("scale"), .01f);
	terrain_shader.setVec3(string("mult"), 5.0f * vec3(3.0,5.0,3.0));

	Shader basic_shader = Shader("src/rendering/shaders/basic");
	glCheckError();


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

	mat4 playerViewMat = lookAt(vec3(0,0,5),vec3(0,0,-100),vec3(0,1.0,0));
	mat4 proj = infinitePerspective(glm::radians(45.0f), 1.0f, 0.01f);
	mat4 rot = mat4(1.0),testmat;
	mat4 trans = translate(vec3(0,.3,-5.0)), trans1 = translate(vec3(0,.3,-9.0)), trans2 = translate(vec3(0,.3,-1.0));

	float sphereMat[16] = {}, boxMat[16] = {};
	

	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		t_start = t_now;

		//mainSimu.stepSimu(time);
		//mainSimu.getModelMats(sphereMat, boxMat);

		//cout << "=========================" << endl;
		//coutMat((boxMat));
		//cout << "-------------------------" << endl;
		//coutMat(value_ptr(trans));
		//cout << "-------------------------" << endl;
		//testmat = trans * rot;
		//coutMat(value_ptr(testmat));
		//cout << "=========================" << endl;

		rot = rotate(rot, time * glm::radians(20.0f), vec3(0.0f,1.0f,0.0f));
		testmat = trans * rot;


		terrain_shader.setMats(value_ptr(testmat), value_ptr(playerViewMat), value_ptr(proj));
		terrain_plane.draw(terrain_shader.progID);

		plane_shader.setMats(value_ptr(testmat), value_ptr(playerViewMat), value_ptr(proj));
		plane.draw(plane_shader.progID);

		testmat = trans1 * rot;

		basic_shader.setMats(value_ptr(testmat), value_ptr(playerViewMat), value_ptr(proj));
		sphere.draw(basic_shader.progID);

		testmat = trans2 * rot;

		basic_shader.setMats(value_ptr(testmat), value_ptr(playerViewMat), value_ptr(proj));
		cube.draw(basic_shader.progID);



		//glUniformMatrix4fv(rotloc, (GLuint) 1, GL_FALSE, boxMat);// value_ptr(transpose(dest)));

		//glCheckError();
		//cube -> draw();

		//memcpy(value_ptr(dest),sphereMat,16 * sizeof(float));

		//glUniformMatrix4fv(rotloc, (GLuint) 1, GL_FALSE, sphereMat);//value_ptr(transpose(dest)));
		//glCheckError();
		
		//sphere -> draw();
		playerViewMat = computeMatricesFromInputs();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
	

	glfwTerminate();

	return 0;
}