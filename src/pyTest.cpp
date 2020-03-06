#include <pybind11/pybind11.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include "rendering/Texture.h"
#include <mesh/Mesh.h>
#include <phys/Physics.h>
#include <gtx/transform.hpp>
#include <perlin/PerlinNoise.hpp>
#include "rendering/Shader.h"
#include "utils/controls.hpp"
#include "rendering/RenderTarget.h"


using namespace std;

void coutMat(float *mat){

	cout << "{" << mat[0] << ", " << mat[1] << ", " << mat[2] << ", " << mat[3] << "," << endl;
	cout << " " << mat[4] << ", " << mat[5] << ", " << mat[6] << ", " << mat[7] << "," << endl;
	cout << " " << mat[8] << ", " << mat[9] << ", " << mat[10] << ", " << mat[11] << "," << endl;
	cout << " " << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << "}" << endl;
}

void coutVec4(vec4 v){
	cout << "{" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << "}" << endl;
}

float *generate_terrain(int dim, double freq, float height_mult, int32_t *physx_samples){

	const siv::PerlinNoise perlin(61881);
	
	float *result = (float *) calloc(dim * dim, sizeof(float));

	double mult = freq/dim;

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){

			float per = (float) perlin.accumulatedOctaveNoise2D_0_1(j * mult, i * mult, 8);

			if (i == 0 || i == dim - 1) per = 0.0;
			if (j == 0 || j == dim - 1) per = 0.0;
			
			result[j * dim + i] = per; //has to be flipped due opengl flipping textures.... I think?

			int32_t cur = (int32_t) 3000.0 * height_mult * per;
			physx_samples[i * dim + j] = cur << 16;
		}
	}
	return result;
}
//Python 3.7 root
///usr/local/opt/python/Frameworks/Python.framework/Versions/

//B2 executable
//Users/will/projects/cpprtx/libs/boost/b2install/bin/b2



//installation path
//Users/will/projects/cpprtx/libs/boost


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
/*
void showFPS(GLFWwindow *pWindow){

	// Measure speed
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	nbFrames++;
	
	if ( delta >= 1.0 ){ // If last cout was more than 1 sec ago
		cout << 1000.0/double(nbFrames) << endl;

		double fps = double(nbFrames) / delta;

		std::stringstream ss;
		ss << GAME_NAME << " " << VERSION << " [" << fps << " FPS]";

		glfwSetWindowTitle(pWindow, ss.str().c_str());

		nbFrames = 0;
		lastTime = currentTime;
	}
}
*/


GLFWwindow* window;


void run_game(){
	
	Simu mainSimu;

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

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwSetKeyCallback(window, key_callback);

	Mesh cube = Mesh(string("/Users/will/projects/cpprtx/assets/meshes/cube.obj"));
	Mesh sphere = Mesh(string("/Users/will/projects/cpprtx/assets/meshes/ball.obj"));
	Mesh terrain_plane = Mesh(string("/Users/will/projects/cpprtx/assets/meshes/terrain_plane.obj"));
	Mesh plane = gen_plane();

	RenderTarget *shadowTarget = new RenderTarget(1024,1024,1);

	//Perlin noise params
	int dim = 64;
	double freq = 3.0;
	int32_t *px_samples = (int32_t *) calloc(dim * dim, sizeof(int32_t));
	vec3 terrain_mult = 3.0f * vec3(10.0f, 5.0f, 10.0f);

	//Img_data is OpenGL texture data itself
    float *img_data = generate_terrain(dim, freq, terrain_mult.y, px_samples);
    Texture noise_tex = Texture(img_data, dim, dim, 0);

    cout << "Reading grass tex" << endl;
    Texture grass_tex = Texture(string("assets/images/grass.jpg"), 0);
    
    cout << "Reading skybox tex" << endl;
    Texture skybox = Texture(string("assets/images/yellowcloud"), 1);
    
    //Texture skybox = Texture(string("/Users/will/projects/TombVoyage/Assets/SpaceSkiesFree/Skybox_2/Textures/1K_Resolution/1K_TEX"), 1);

    Shader shadow_shader = Shader("src/rendering/shaders/shadow");
	Shader plane_shader = Shader("src/rendering/shaders/plane");	
	Shader terrain_shader = Shader("src/rendering/shaders/noise_test");
	Shader basic_shader = Shader("src/rendering/shaders/basic");
	Shader skybox_shader = Shader("src/rendering/shaders/cubemap");
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

	mat4 playerViewMat = lookAt(vec3(18,18,18),vec3(0,0,-10),vec3(0,1.0,0));
	mat4 proj = infinitePerspective(glm::radians(45.0f), 1.0f, 1.0f);
	//mat4 proj = perspective(glm::radians(45.0f), 1.0f, 0.01f, 30.0f);
	mat4 rot = mat4(1.0),testmat;
	mat4 iden = mat4(1.0);
	mat4 trans = translate(vec3(0,9,0));
	mat4 lighttrans = translate(vec3(0,18,18));

	vec3 lightPos = vec3(0,18,18);
	vec3 lookDir = vec3(0,9,0) - lightPos;

	mat4 depthOrtho = proj;// ortho<float>(-10,10,-10,10,0,20);
 	mat4 depthView = lookAt(lightPos, vec3(0,9,0), normalize( cross(vec3(1,0,0),lookDir) ));

	mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

	mat4 depthProjMat = glm::ortho<float>(-20,20,-10,10,0,30);

	vec3 tm = terrain_mult;

	mat t1 = translate(vec3(-1,0,-1) * tm), t2 = translate(vec3(1,0,-1) * tm), t3 = translate(vec3(-1,0,1) * tm), t4 = translate(vec3(1,0,1) * tm);
	float *t1p = value_ptr(t1), *t2p = value_ptr(t2), *t3p = value_ptr(t3), *t4p = value_ptr(t4);
	
	float sphereMat[16] = {}, boxMat[16] = {}; 
	float *viewptr = value_ptr(playerViewMat), *projptr = value_ptr(proj);
	
	mainSimu.addTerrain(px_samples, dim, terrain_mult);
	mainSimu.addSphere(vec3(-9,13,-9), 1.0f, 1);
	mainSimu.addCube(vec3(6,15,6), 1.0f, 2);

	basic_shader.setProj(projptr);

	terrain_shader.setProj(projptr);
	terrain_shader.setDataTexture(noise_tex.getID(), 6);
	terrain_shader.setImageTexture(grass_tex.getID(), 0, 8);
	terrain_shader.setFloat(string("dim"), dim);
	terrain_shader.setVec3(string("mult"), terrain_mult);
	terrain_shader.setMat4(string("shadowBias"), biasMatrix);
	terrain_shader.setMat4(string("shadowView"), depthView);
 	terrain_shader.setMat4(string("shadowProj"), depthProjMat);
    terrain_shader.setShadowTexture(shadowTarget -> getTexture());

	plane_shader.setImageTexture(shadowTarget -> getTexture(), 0, 4);
	plane_shader.setProj(projptr);

	skybox_shader.setProj(projptr);
	skybox_shader.setImageTexture(skybox.getID(), 1, 9);

 	shadow_shader.setProj(value_ptr(depthProjMat));
 	shadow_shader.setView(value_ptr(depthView));


	do {

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		t_start = t_now;

		mainSimu.stepSimu(time);
		mainSimu.getModelMats(sphereMat, boxMat);

		rot = rotate(rot, time * glm::radians(20.0f), vec3(0.0f,1.0f,0.0f));
		testmat = trans * rot;

		shadowTarget -> set();
		shadow_shader.setModel(value_ptr(testmat));
		plane.draw(shadow_shader.progID);
		shadow_shader.setModel(sphereMat);
		sphere.draw(shadow_shader.progID);
		shadow_shader.setModel(boxMat);
		cube.draw(shadow_shader.progID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		terrain_shader.setView(viewptr);
		terrain_shader.setProj(projptr);
		terrain_plane.draw(terrain_shader.progID);

		plane_shader.setModel(value_ptr(testmat));
		plane_shader.setView(viewptr);
		plane.draw(plane_shader.progID);


		basic_shader.setView(viewptr);
		basic_shader.setModel(sphereMat);
		basic_shader.setColor(vec3(1.0,0.0,0.0));
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(boxMat);
		basic_shader.setColor(vec3(.0,0.0,1.0));
		cube.draw(basic_shader.progID);


		basic_shader.setModel(t1p);
		basic_shader.setColor(2.0f*vec3(1.0,0.0,0.0));
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(t2p);
		basic_shader.setColor(2.0f*vec3(0.0,1.0,0.0));
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(t3p);
		basic_shader.setColor(2.0f*vec3(0.0,0.0,1.0));
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(value_ptr(lighttrans));
		basic_shader.setColor(2.0f*vec3(1.0,1.0,0.0));
		sphere.draw(basic_shader.progID);

		skybox_shader.setView(viewptr);
		cube.draw(skybox_shader.progID);
		glCheckError();


		playerViewMat = computeMatricesFromInputs();
		viewptr = value_ptr(playerViewMat);	


		glfwSwapBuffers(window);
		glfwPollEvents();
	} 

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	delete shadowTarget;
	delete px_samples;
}


PYBIND11_MODULE(gltest, m) {
    m.doc() = "God help us";
    m.def("run_game", &run_game, "Run EVERYTHING");
}