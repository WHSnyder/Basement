#ifdef PYBINDMAIN
#include <pybind11/pybind11.h>
#endif


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
#include "imgui/imgui.h"
#include "imgui/bindings/imgui_impl_glfw.h"
#include "imgui/bindings/imgui_impl_opengl3.h"
*/

#include "GameContext.h"

#include "rendering/Texture.h"
#include "mesh/Mesh.h"
#include "phys/Physics.h"
#include "glm/gtx/transform.hpp"
#include "perlin/PerlinNoise.hpp"
#include "utils/Controls.hpp"
#include "rendering/RenderTarget.h"
#include "tf_driver/StyleTransfer.h"

#include "clouds/Clouds.h"

#define COUT(x) std::cout << x << std::endl;




#ifdef MACOS
//#include "rendering/GLMetalInteropTex.h"
//#define GLFW_EXPOSE_NATIVE_NSGL
#endif

#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>


GLuint64 startTime, stopTime;
unsigned int queryID[2];    
std::unique_ptr<Texture> outputTexture;  

float CURTIME, TIMESTEP;
glm::mat4 VIEWMAT = lookAt(vec3(18,18,18),vec3(0,0,-10),vec3(0,1.0,0));
glm::mat4 PROJMAT = infinitePerspective(glm::radians(45.0f), 1.0f, 1.0f);
glm::vec3 POSITION;
int SCR_WIDTH, SCR_HEIGHT;
int userIn=0;



void setTimer(){
	
	glGenQueries(2, queryID);
	glQueryCounter(queryID[0], GL_TIMESTAMP);
}


GLuint64 getTimer(int block){

	glQueryCounter(queryID[1], GL_TIMESTAMP);

	GLuint64 stopTimerAvailable = 0;
	int tc = 0;
	while (!stopTimerAvailable && block) {
	  glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
	  tc++;
	}

	glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
	glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &stopTime);

	return stopTime - startTime;
}




using namespace std;

GLuint ssboOut, ssboIn;
int terrainSeed;

string basepath;
int inputbreak;
int toggleNetwork;


const char *glsl_version = "#version 450";

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

	const siv::PerlinNoise perlin((float)terrainSeed);
	
	float *result = (float *) calloc(dim * dim, sizeof(float));

	double mult = freq/dim;

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){

			//This lib will be replaced by shader implementation (readback of course for baking)
			float per = (float) perlin.accumulatedOctaveNoise2D_0_1(j * mult, i * mult, 8);

			if (i == 0 || i == dim - 1) per = 0.0; 
			if (j == 0 || j == dim - 1) per = 0.0;
			
			//Texture flipped for opengl <-> physx correspondence
			result[j * dim + i] = per; 

			//Increase range of samples, then set scale factor in HF constructor to compensate for 16 bit limitation
			int32_t cur = (int32_t) 3000.0 * height_mult * per; 

			//Shift the data left to fit physx HF sample format
			physx_samples[i * dim + j] = cur << 16; 
		}
	}
	return result;
}


//For quads fullscreen or otherwise
unique_ptr<Mesh> gen_plane(){

    vec3 verts[] = {vec3(-1,1,0), vec3(1,1,0), vec3(-1,-1,0), vec3(1,-1,0)};
    vec3 norms[] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1)};
    GLuint inds[] = {0,1,2,1,3,2};

    return make_unique<Mesh>(vector<vec3>(verts,verts+4), vector<vec3>(norms,norms+4), vector<GLuint>(inds,inds+6));
}


static void error_callback(int error, const char* description){
	fputs(description, stderr);
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
   	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
   		toggleNetwork = ~toggleNetwork;
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
   		userIn = 1;
}

double lastTime; 
int nbFrames = 0;

//From stackoverflow @https://gamedev.stackexchange.com/questions/133173/how-to-calculate-fps-in-glfw
void showFPS(GLFWwindow *pWindow){

	double delta = CURTIME - lastTime;
	nbFrames++;
	
	if (delta >= 1.0){ // If last cout was more than 1 sec ago

		double fps = double(nbFrames) / delta;

		std::stringstream ss;
		ss << "GLTest" << " [" << fps << " FPS]";

		glfwSetWindowTitle(pWindow, ss.str().c_str());

		nbFrames = 0;
		lastTime = CURTIME;
	}
}


void textureToSSBO(Shader *tex2ssbo, GLuint texID, GLuint ssbo){

	glUseProgram(tex2ssbo -> progID); 
	tex2ssbo -> setImageTexture(texID, 0, 7);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glDispatchCompute(24, 24, 1);    
 	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}


void SSBOToTexture(Shader *ssbo2tex, GLuint ssbo, GLuint texID){

	glUseProgram(ssbo2tex -> progID); 
	glBindImageTexture(0, texID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
	glDispatchCompute(24, 24, 1);    
 	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
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

	SCR_HEIGHT = height;
	SCR_WIDTH = width;

	return 0;
}





std::unique_ptr<StyleTransfer> stModel;

std::unique_ptr<RenderTarget> shadowTarget, textureTarget, cloudTarget;
std::unique_ptr<Simu> mainSimu;
std::unique_ptr<Mesh> cube, sphere, terrain_plane, plane;
std::unique_ptr<Texture> noise_tex, grass_tex, skybox, scream;
std::unique_ptr<Shader> shadow_shader, terrain_shader, basic_shader, skybox_shader, plane_shader, tex2SSBO, SSBO2tex;

std::unique_ptr<Clouds> clouds;


//Perlin noise params
int dim = 64;
double freq = 3.0;
int32_t *px_samples = (int32_t *) calloc(dim * dim, sizeof(int32_t));
vec3 terrain_mult = 3.0f * vec3(10.0f, 5.0f, 10.0f);

//Texture data
float *img_data;// = generate_terrain(dim, freq, terrain_mult.y, px_samples);

float sphereMat[16] = {}, boxMat[16] = {}; 


mat4 rot = mat4(1.0), testmat, iden = mat4(1.0), trans = translate(vec3(0,9,0)), lighttrans = translate(vec3(0,18,18));

vec3 lightPos = vec3(0,18,18), lookDir = vec3(0,9,0) - lightPos;

mat4 depthView = lookAt(lightPos, vec3(0,9,0), normalize(cross(vec3(1,0,0),lookDir)));
mat4 depthProjMat = glm::ortho<float>(-20,20,-10,10,0,30);

vec3 ter_mult = terrain_mult;

mat t1 = translate(vec3(-1,0,-1) * ter_mult), t2 = translate(vec3(1,0,-1) * ter_mult), t3 = translate(vec3(-1,0,1) * ter_mult), t4 = translate(vec3(1,0,1) * ter_mult);
float *t1p = value_ptr(t1), *t2p = value_ptr(t2), *t3p = value_ptr(t3), *t4p = value_ptr(t4);

float *viewptr = value_ptr(VIEWMAT), *projptr = value_ptr(PROJMAT);
int bufferSize = 600 * 600 * 3;
float *dataOutSSBO, *dataInSSBO;




int initialize_game(string inpath){

	cloudTarget = make_unique<RenderTarget>(384,384,0);
	clouds = make_unique<Clouds>(384,384);

	img_data = generate_terrain(dim, freq, terrain_mult.y, px_samples);

	//This SSBO will hold the ST's output
	dataOutSSBO = new float[bufferSize]();

	glGenBuffers(1, &ssboOut);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize*sizeof(float), dataOutSSBO, GL_DYNAMIC_COPY);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(p, dataOutSSBO, bufferSize * sizeof(float));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	//We will render to textureTarget, then copy to this SSBO w/compute
	//shader.  ST then consumes this SSBO.
	dataInSSBO = new float[bufferSize]();

	glGenBuffers(1, &ssboIn);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIn);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize*sizeof(float), dataInSSBO, GL_DYNAMIC_COPY);
	GLvoid* p2 = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(p2, dataInSSBO, bufferSize * sizeof(float));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	//Create and set style for the ST
	stModel = make_unique<StyleTransfer>(ssboOut,ssboIn);
	if (stModel == nullptr)
		return -1;
	stModel -> setStyle(0);
	stModel -> prime();

	basepath = inpath;
	mainSimu = make_unique<Simu>();
	
	cube = make_unique<Mesh>(string("assets/meshes/cube.obj"));
	sphere =  make_unique<Mesh>(string("assets/meshes/ball.obj"));
	terrain_plane = make_unique<Mesh>(string("assets/meshes/terrain_plane.obj"));
	plane = gen_plane();

	shadowTarget = make_unique<RenderTarget>(1024,1024,1);
	//textureTarget = new RenderTarget(200,200,0);

	outputTexture = make_unique<Texture>(384,384);

	noise_tex = make_unique<Texture>(img_data, dim, dim, 0);
    grass_tex = make_unique<Texture>(string("assets/images/volcanic.jpg"), 0);
    skybox = make_unique<Texture>(string("assets/images/yellowcloud"), 1);
    scream = make_unique<Texture>(string("assets/images/scream.jpg"), 0);
    //Texture skybox = Texture(string("/Users/will/projects/TombVoyage/Assets/SpaceSkiesFree/Skybox_2/Textures/1K_Resolution/1K_TEX"), 1);

    shadow_shader = make_unique<Shader>("assets/shaders/shadow"); 
	plane_shader = make_unique<Shader>("assets/shaders/plane");
	terrain_shader = make_unique<Shader>("assets/shaders/noise_test");
	basic_shader = make_unique<Shader>("assets/shaders/basic");
	skybox_shader = make_unique<Shader>("assets/shaders/cubemap");
	tex2SSBO = make_unique<Shader>("assets/shaders/texRGB_2_ssbo",1);
	
	//Replaced by modified plane shader that covers screen
	SSBO2tex = make_unique<Shader>("assets/shaders/ssbo_2_texRGB",1);

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

	skybox_shader -> setProj(projptr);
	skybox_shader -> setImageTexture(skybox -> getID(), 1, 9);

 	shadow_shader -> setProj(value_ptr(depthProjMat));
 	shadow_shader -> setView(value_ptr(depthView));

 	textureTarget = make_unique<RenderTarget>(384,384,0);

 	plane_shader -> setModel(value_ptr(iden));
	plane_shader -> setView(value_ptr(iden));
	plane_shader -> setProj(value_ptr(iden));

	return 0;
}


//Run main game loop
int step_game(float timestep){

	CURTIME += timestep;

	/*
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Triangle Position/Color");
	static float rotation = 0.0;
	ImGui::SliderFloat("rotation", &rotation, 0, 2 * 3);
	ImGui::End();
	*/

	VIEWMAT = computeMatricesFromInputs();
	viewptr = value_ptr(VIEWMAT);	
	
	mainSimu -> stepSimu(timestep);
	mainSimu -> getModelMats();

	rot = rotate(rot, timestep * glm::radians(20.0f), vec3(0.0f,1.0f,0.0f));
	testmat = trans * rot;

	//setTimer();
	shadowTarget -> set();
	
	shadow_shader -> setModel(sphereMat);
	sphere -> draw(shadow_shader -> progID);
	shadow_shader -> setModel(boxMat);
	cube -> draw(shadow_shader -> progID);

	//terrain_shader -> setProj(value_ptr(depthProjMat));
 	//terrain_shader -> setView(value_ptr(depthView));
	//terrain_plane -> draw(terrain_shader -> progID);

	glCheckError();


	textureTarget -> set();

	clouds -> update();
	clouds -> draw(textureTarget -> getTexture());

	//plane_shader -> setImageTexture(cloudTarget -> getTexture(),0,5);
	//plane -> draw(plane_shader -> progID);

	terrain_shader -> setProj(projptr);
	terrain_shader -> setView(viewptr);
	terrain_plane -> draw(terrain_shader -> progID);

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

	//skybox_shader -> setView(viewptr);
	//cube -> draw(skybox_shader -> progID);

	textureToSSBO(tex2SSBO.get(), textureTarget -> getTexture(), ssboIn);
	
	
	if (toggleNetwork){
		stModel -> execute();
		SSBOToTexture(SSBO2tex.get(), ssboOut, outputTexture -> getID());
	} else {
		SSBOToTexture(SSBO2tex.get(), ssboIn, outputTexture -> getID());
	}
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	plane_shader -> setImageTexture(outputTexture -> getID(),0,5);
	plane -> draw(plane_shader -> progID);

	/*
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	showFPS(window);*/

	//printf("GPU FPS: %lu\n", 1000 / (getTimer(1) / 1000000));

	

	int x;
	if (userIn){
		userIn = 0;
		std::cout << "Yes? ";
		std::cin >> x;
		std::cout << "Number entered: " << x << std::endl;
	}

	glfwSwapBuffers(window);
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0)
		return -1;
	return 0;
}


void destroy_context(){
	glfwDestroyWindow(window);
	glfwTerminate();	
}


void destroy_game(){
	
	/*
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	*/	

	delete px_samples;
	
	glDeleteBuffers(1,&ssboOut);
	glDeleteBuffers(1,&ssboIn);

	delete[] dataOutSSBO;
	delete[] dataInSSBO;

	destroy_context();
}


#ifndef PYBINDMAIN 

int main(int argc, char **argv){

	COUT("GAME LAUNCH")
//bazel build --config "linux" -c opt --copt "-Os" --copt "-DMAC_OPENGL" --copt "-DTFLITE_GPU_BINARY_RELEASE" --verbose_failures :libtensorflowlite_gpu_delegate.so 
#if __linux__
	std::string inpath = "/home/will/projects/cpprtx/";
#elif __APPLE__
	std::string inpath = "/Users/will/projects/cpprtx/";
#endif

	initialize_window();

	auto p0 = std::chrono::time_point<std::chrono::system_clock>{};
	//auto epoch_time = std::chrono::system_clock::to_time_t(p0);

	auto t_last = std::chrono::high_resolution_clock::now();
	
	srand(time(NULL));
    terrainSeed = rand() % 100000000 + 1; 
	img_data = generate_terrain(dim, freq, terrain_mult.y, px_samples);

	if (initialize_game(inpath) == -1){
		COUT("GAME INIT ERROR")
		return -1;
	}

	auto t_now = t_last;
	float timeStep;

	while (1){

		t_now = std::chrono::high_resolution_clock::now();
		timeStep = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_last).count();
		t_last = t_now;

		step_game(timeStep);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0)
			break;
	}

	destroy_game();
	//exit(0);
	return 0;
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