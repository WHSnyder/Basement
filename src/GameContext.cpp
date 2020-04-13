#ifdef PYBIND
#include <pybind11/pybind11.h>
#endif

//#ifdef GUI
#include "imgui/imgui.h"
#include "imgui/bindings/imgui_impl_glfw.h"
#include "imgui/bindings/imgui_impl_opengl3.h"
//#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include "rendering/Texture.h"
#include "mesh/Mesh.h"
#include "phys/Physics.h"
#include "glm/gtx/transform.hpp"
#include "perlin/PerlinNoise.hpp"
#include "rendering/Shader.h"
#include "utils/Controls.hpp"
#include "rendering/RenderTarget.h"


using namespace std;

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

//Not for generating full screen quad!
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

double lastTime; 
int nbFrames = 0;

//From stackoverflow @
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

auto t_start = std::chrono::high_resolution_clock::now();
auto t_now = std::chrono::high_resolution_clock::now();
float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

GLFWwindow* window;

void initialize_window(){

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

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
}

Simu *mainSimu;

Mesh cube, sphere, terrain_plane, plane;
RenderTarget *shadowTarget;
Texture noise_tex, grass_tex, skybox;
Shader shadow_shader, plane_shader, terrain_shader, basic_shader, skybox_shader;

//Perlin noise params
int dim = 64;
double freq = 3.0;
int32_t *px_samples = (int32_t *) calloc(dim * dim, sizeof(int32_t));
vec3 terrain_mult = 3.0f * vec3(10.0f, 5.0f, 10.0f);

//Img_data is OpenGL texture data itself
float *img_data = generate_terrain(dim, freq, terrain_mult.y, px_samples);

float sphereMat[16] = {}, boxMat[16] = {}; 

mat4 playerViewMat = lookAt(vec3(18,18,18),vec3(0,0,-10),vec3(0,1.0,0));
mat4 proj = infinitePerspective(glm::radians(45.0f), 1.0f, 1.0f);
mat4 rot = mat4(1.0), testmat, iden = mat4(1.0), trans = translate(vec3(0,9,0)), lighttrans = translate(vec3(0,18,18));

vec3 lightPos = vec3(0,18,18), lookDir = vec3(0,9,0) - lightPos;

mat4 depthOrtho = proj;
mat4 depthView = lookAt(lightPos, vec3(0,9,0), normalize(cross(vec3(1,0,0),lookDir)));
mat4 depthProjMat = glm::ortho<float>(-20,20,-10,10,0,30);

vec3 tm = terrain_mult;

mat t1 = translate(vec3(-1,0,-1) * tm), t2 = translate(vec3(1,0,-1) * tm), t3 = translate(vec3(-1,0,1) * tm), t4 = translate(vec3(1,0,1) * tm);
float *t1p = value_ptr(t1), *t2p = value_ptr(t2), *t3p = value_ptr(t3), *t4p = value_ptr(t4);

float *viewptr = value_ptr(playerViewMat), *projptr = value_ptr(proj);


void initialize_game(){

	mainSimu = new Simu();
	
	cube = Mesh(string("assets/meshes/cube.obj"));
	sphere = Mesh(string("assets/meshes/ball.obj"));
	terrain_plane = Mesh(string("assets/meshes/terrain_plane.obj"));
	plane = gen_plane();

	shadowTarget = new RenderTarget(1024,1024,1);

	noise_tex = Texture(img_data, dim, dim, 0);
    grass_tex = Texture(string("assets/images/grass.jpg"), 0);
    skybox = Texture(string("assets/images/yellowcloud"), 1);
    //Texture skybox = Texture(string("/Users/will/projects/TombVoyage/Assets/SpaceSkiesFree/Skybox_2/Textures/1K_Resolution/1K_TEX"), 1);

    shadow_shader = Shader("assets/shaders/shadow");
	plane_shader = Shader("assets/shaders/plane");	
	terrain_shader = Shader("assets/shaders/noise_test");
	basic_shader = Shader("assets/shaders/basic");
	skybox_shader = Shader("assets/shaders/cubemap");

	mainSimu -> addTerrain(px_samples, dim, terrain_mult);
	mainSimu -> addSphere(vec3(-9,13,-9), 1.0f, 1, reinterpret_cast<void *>(sphereMat));
	mainSimu -> addCube(vec3(6,15,6), 1.0f, 2, reinterpret_cast<void *>(boxMat));

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

	lastTime = glfwGetTime();
}






//Run main game loop
int run_game(){

	do {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Triangle Position/Color");
		static float rotation = 0.0;
		ImGui::SliderFloat("rotation", &rotation, 0, 2 * 3);
		ImGui::End();

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		t_start = t_now;

		mainSimu -> stepSimu(time);
		mainSimu -> getModelMats();

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


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//showFPS(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	return 0;
}


void destroy_game(){

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	delete shadowTarget;
	delete px_samples;
	delete mainSimu;
}


#ifndef PYBIND 

int main(int argc, char **argv){
	initialize_window();
	run_game();
	return 1;
}

#else

PYBIND11_MODULE(GameContext, m) {
    m.doc() = "Full game loop";
    m.def("run_game", &run_game, "Run everything");
    m.def("initialize_window", &initialize_window, "Initialize imgui, windows, etc");
    m.def("initialize_game", &initialize_game, "Initialize game resources, vars");
    m.def("step_game", &step_game, "Run game iteration")
}

#endif