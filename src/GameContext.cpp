#ifdef PYBINDMAIN
#include <pybind11/pybind11.h>
#endif

/*
#include "imgui/imgui.h"
#include "imgui/bindings/imgui_impl_glfw.h"
#include "imgui/bindings/imgui_impl_opengl3.h"
*/

#include <GL/glew.h>

#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include <vector>

#include <sys/types.h>
#include <unistd.h>

#include "rendering/Texture.h"
#include "mesh/Mesh.h"
#include "phys/Physics.h"
#include "glm/gtx/transform.hpp"
#include "perlin/PerlinNoise.hpp"
#include "rendering/Shader.h"
#include "utils/Controls.hpp"
#include "rendering/RenderTarget.h"
#include "tf_driver/StyleTransfer.h"

#include <GLFW/glfw3.h>


using namespace std;




string basepath;
int inputbreak = 0;


const char *glsl_version = "#version 410";

mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);


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

			//This lib will be replaced by shader implementation (readback of course for baking)
			float per = (float) perlin.accumulatedOctaveNoise2D_0_1(j * mult, i * mult, 8);

			if (i == 0 || i == dim - 1) per = 0.0; 
			if (j == 0 || j == dim - 1) per = 0.0;
			
			//Texture be flipped for opengl <-> physx correspondence
			result[j * dim + i] = per; 

			//Increase range of samples, then set scale factor in HF constructor to compensate for 16 bit limitation
			int32_t cur = (int32_t) 3000.0 * height_mult * per; 

			//Shift the data left to fit physx HF sample format
			physx_samples[i * dim + j] = cur << 16; 
		}
	}
	return result;
}

//Python 3.7 root
///usr/local/opt/python/Frameworks/Python.framework/Versions/

//For quads fullscreen or otherwise
Mesh *gen_plane(){

    vec3 verts[] = {vec3(-1,1,0), vec3(1,1,0), vec3(-1,-1,0), vec3(1,-1,0)};
    vec3 norms[] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1)};
    GLuint inds[] = {0,1,2,1,3,2};

    return new Mesh(vector<vec3>(verts,verts+4), vector<vec3>(norms,norms+4), vector<GLuint>(inds,inds+6));
}


static void error_callback(int error, const char* description){
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
   	if (key == GLFW_KEY_Q)
   		inputbreak = 1;
}

double lastTime; 
int nbFrames = 0;

//From stackoverflow @https://gamedev.stackexchange.com/questions/133173/how-to-calculate-fps-in-glfw
void showFPS(GLFWwindow *pWindow){

	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	nbFrames++;
	
	if ( delta >= 1.0 ){ // If last cout was more than 1 sec ago

		double fps = double(nbFrames) / delta;

		std::stringstream ss;
		ss << "GLTest" << " [" << fps << " FPS]";

		glfwSetWindowTitle(pWindow, ss.str().c_str());

		nbFrames = 0;
		lastTime = currentTime;
	}
}


float timeratio;
int width, height;

GLFWwindow* window;

int initialize_window(){

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(600, 600, "Test", NULL, NULL);
	if (!window) {
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

    glfwGetFramebufferSize(window, &width, &height);
    timeratio = width / (float) height;
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	/*IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();*/
	return 0;
}


Simu *mainSimu;

Mesh *cube, *sphere, *terrain_plane, *plane;
RenderTarget *shadowTarget, *textureTarget;
Texture *noise_tex, *grass_tex, *skybox;
Shader *shadow_shader, *terrain_shader, *basic_shader, *skybox_shader, *plane_shader, *copy_compute_shader;

//Perlin noise params
int dim = 64;
double freq = 3.0;
int32_t *px_samples = (int32_t *) calloc(dim * dim, sizeof(int32_t));
vec3 terrain_mult = 3.0f * vec3(10.0f, 5.0f, 10.0f);

//Texture data
float *img_data = generate_terrain(dim, freq, terrain_mult.y, px_samples);

float sphereMat[16] = {}, boxMat[16] = {}; 

mat4 playerViewMat = lookAt(vec3(18,18,18),vec3(0,0,-10),vec3(0,1.0,0));
mat4 proj1 = infinitePerspective(glm::radians(45.0f), 1.0f, 1.0f);
mat4 rot = mat4(1.0), testmat, iden = mat4(1.0), trans = translate(vec3(0,9,0)), lighttrans = translate(vec3(0,18,18));

vec3 lightPos = vec3(0,18,18), lookDir = vec3(0,9,0) - lightPos;

mat4 depthOrtho = proj1;
mat4 depthView = lookAt(lightPos, vec3(0,9,0), normalize(cross(vec3(1,0,0),lookDir)));
mat4 depthProjMat = glm::ortho<float>(-20,20,-10,10,0,30);

vec3 ter_mult = terrain_mult;

mat t1 = translate(vec3(-1,0,-1) * ter_mult), t2 = translate(vec3(1,0,-1) * ter_mult), t3 = translate(vec3(-1,0,1) * ter_mult), t4 = translate(vec3(1,0,1) * ter_mult);
float *t1p = value_ptr(t1), *t2p = value_ptr(t2), *t3p = value_ptr(t3), *t4p = value_ptr(t4);

float *viewptr = value_ptr(playerViewMat), *projptr = value_ptr(proj1);


void initialize_game(string inpath){

	basepath = inpath;

	mainSimu = new Simu();
	
	cube = new Mesh(string("assets/meshes/cube.obj"));
	sphere = new Mesh(string("assets/meshes/ball.obj"));
	terrain_plane = new Mesh(string("assets/meshes/terrain_plane.obj"));
	plane = gen_plane();

	shadowTarget = new RenderTarget(1024,1024,1);
	textureTarget = new RenderTarget(200,200,0);

	noise_tex = new Texture(img_data, dim, dim, 0);
    grass_tex = new Texture(string("assets/images/grass.jpg"), 0);
    skybox = new Texture(string("assets/images/yellowcloud"), 1);
    //Texture skybox = Texture(string("/Users/will/projects/TombVoyage/Assets/SpaceSkiesFree/Skybox_2/Textures/1K_Resolution/1K_TEX"), 1);

    shadow_shader = new Shader("assets/shaders/shadow"); 
	plane_shader = new Shader("assets/shaders/plane");
	terrain_shader = new Shader("assets/shaders/noise_test");
	basic_shader = new Shader("assets/shaders/basic");
	skybox_shader = new Shader("assets/shaders/cubemap");

	copy_compute_shader = new Shader("assets/shaders/texRGB_2_ssbo",1);

	mainSimu -> addTerrain(px_samples, dim, terrain_mult);
	mainSimu -> addSphere(vec3(-4,23,-4), 1.0f, 1, reinterpret_cast<void *>(sphereMat));
	mainSimu -> addCube(vec3(-8,27,-8.5), 1.0f, 2, reinterpret_cast<void *>(boxMat));

	basic_shader -> setProj(projptr);

	terrain_shader -> setProj(projptr);
	terrain_shader -> setDataTexture(noise_tex -> getID(), 6);
	terrain_shader -> setImageTexture(grass_tex -> getID(), 0, 8);
	terrain_shader -> setFloat(string("dim"), dim);
	terrain_shader -> setVec3(string("mult"), terrain_mult);
	terrain_shader -> setMat4(string("shadowBias"), biasMatrix);
	terrain_shader -> setMat4(string("shadowView"), depthView);
 	terrain_shader -> setMat4(string("shadowProj"), depthProjMat);
    terrain_shader -> setShadowTexture(shadowTarget -> getTexture());

	plane_shader -> setImageTexture(shadowTarget -> getTexture(), 0, 4);
	plane_shader -> setProj(projptr);

	skybox_shader -> setProj(projptr);
	skybox_shader -> setImageTexture(skybox -> getID(), 1, 9);

 	shadow_shader -> setProj(value_ptr(depthProjMat));
 	shadow_shader -> setView(value_ptr(depthView));
}


//Run main game loop
int step_game(float timestep){

	/*ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Triangle Position/Color");
	static float rotation = 0.0;
	ImGui::SliderFloat("rotation", &rotation, 0, 2 * 3);
	ImGui::End();*/

	mainSimu -> stepSimu(timestep);
	mainSimu -> getModelMats();

	rot = rotate(rot, timestep * glm::radians(20.0f), vec3(0.0f,1.0f,0.0f));
	testmat = trans * rot;

	shadowTarget -> set();
	shadow_shader -> setModel(value_ptr(testmat));
	plane -> draw(shadow_shader -> progID);
	shadow_shader -> setModel(sphereMat);
	sphere -> draw(shadow_shader -> progID);
	shadow_shader -> setModel(boxMat);
	cube -> draw(shadow_shader -> progID);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCheckError();

	terrain_shader -> setView(viewptr);
	terrain_shader -> setProj(projptr);
	terrain_plane -> draw(terrain_shader -> progID);

	plane_shader -> setModel(value_ptr(testmat));
	plane_shader -> setView(viewptr);
	plane -> draw(plane_shader -> progID);

	basic_shader -> setView(viewptr);
	basic_shader -> setModel(sphereMat);
	basic_shader -> setColor(vec3(1.0,0.0,0.0));
	sphere -> draw(basic_shader -> progID);

	basic_shader -> setModel(boxMat);
	basic_shader -> setColor(vec3(.0,0.0,1.0));
	cube -> draw(basic_shader -> progID);

	basic_shader -> setModel(t1p);
	basic_shader -> setColor(2.0f*vec3(1.0,0.0,0.0));
	sphere -> draw(basic_shader -> progID);

	basic_shader -> setModel(t2p);
	basic_shader -> setColor(2.0f*vec3(0.0,1.0,0.0));
	sphere -> draw(basic_shader -> progID);

	basic_shader -> setModel(t3p);
	basic_shader -> setColor(2.0f*vec3(0.0,0.0,1.0));
	sphere -> draw(basic_shader -> progID);

	basic_shader -> setModel(value_ptr(lighttrans));
	basic_shader -> setColor(2.0f*vec3(1.0,1.0,0.0));
	sphere -> draw(basic_shader -> progID);

	skybox_shader -> setView(viewptr);
	cube -> draw(skybox_shader -> progID);
	glCheckError();

	playerViewMat = computeMatricesFromInputs();
	viewptr = value_ptr(playerViewMat);	

	/*ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	showFPS(window);*/

	glfwSwapBuffers(window);
	glfwPollEvents();

	if (inputbreak)
		inputbreak = 0;
		return 1;
	return 0;
}


void destroy_game(){
	/*
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	*/
	
	glfwDestroyWindow(window);
	glfwTerminate();

	delete shadowTarget;
	delete textureTarget;
	delete px_samples;
	delete mainSimu;

	delete shadow_shader;
	delete plane_shader;
	delete terrain_shader;
	delete skybox_shader;
	delete basic_shader;
	delete copy_compute_shader;

	delete grass_tex;
	delete skybox;
	delete noise_tex;

	delete plane;
	delete sphere;
	delete cube;
	delete terrain_plane;
}


#ifndef PYBINDMAIN 

int main(int argc, char **argv){

#if __APPLE__
	string inpath = "/Users/will/projects/cpprtx/";
#else
	string inpath = "/home/will/projects/cpprtx/";
#endif
	
	initialize_window();
	initialize_game(inpath);

	auto t_last = std::chrono::high_resolution_clock::now();
	auto t_now = t_last;
	float curtime = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_last).count();

	while (1){

		t_now = std::chrono::high_resolution_clock::now();
		curtime = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_last).count();
		t_last = t_now;

		step_game(curtime);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0)
			break;
	}

	destroy_game();
	return 1;
}


#else

PYBIND11_MODULE(GameContext, m) {
    
    m.doc() = "Full game loop";
    m.def("run_model", &run_model, "Run CPP test for TF");
    m.def("init_window", &initialize_window, "Initialize imgui, windows, etc");
    m.def("init_game", &initialize_game, "Initialize game resources, vars");
    m.def("step_game", &step_game, "Run game iteration");
    m.def("destroy_game", &destroy_game, "Run game iteration");
}

#endif