#include "clouds/Clouds.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <random>

//#define TIMETO(CODE, TASK) 	t1 = glfwGetTime(); CODE; t2 = glfwGetTime(); std::cout << "Time to " + std::string(TASK) + " :" << (t2 - t1)*1e3 << "ms" << std::endl;


extern glm::vec3 POSITION;
int scr_WIDTH, scr_HEIGHT;
extern float CURTIME;
extern glm::mat4 VIEWMAT,PROJMAT;

extern GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 


int counter = 0;


using namespace glm;
using namespace std;



glm::vec3 genRandomVec3() {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(.0, 100.);

	float x, y, z;
	x = dis(gen);
	y = dis(gen);
	z = dis(gen);

	return glm::vec3(x, y, z);
}


Clouds::Clouds(int w, int h) {
	scr_WIDTH = w;
	scr_HEIGHT = h;
	initVariables();
	initShaders();
	generateModelTextures();
}


void Clouds::initShaders() {
	
	cloudsShader = make_unique<Shader>("assets/shaders/volumetric_clouds",1);
	perlinWorelyShader = make_unique<Shader>("assets/shaders/perlin_worely",1);
	worleyShader = make_unique<Shader>("assets/shaders/worely",1);
	weatherShader = make_unique<Shader>("assets/shaders/weather",1);

	glCheckError();

	//postProcessingShader = new ScreenSpaceShader("shaders/clouds_post.frag");
}


void Clouds::generateModelTextures() {
				
	perlinTex = make_unique<Texture>(128, 128, 128);
	
	//perlinWorelyShader -> setVec3("u_resolution", vec3(128, 128, 128));
	//perlinWorelyShader -> setInt("outVolTex", 0);

	glUseProgram(perlinWorelyShader -> progID); //Need to abstract that...
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, perlinTex -> getID());
  	glBindImageTexture(0, perlinTex -> getID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glDispatchCompute(INT_CEIL(128, 4), INT_CEIL(128, 4), INT_CEIL(128, 4));
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glCheckError();
	
	COUT("Computed perlin")
	glGenerateMipmap(GL_TEXTURE_3D);

	
	worleyTex = make_unique<Texture>(32, 32, 32);
	
	glUseProgram(worleyShader -> progID);
	glActiveTexture(GL_TEXTURE0);
  	glBindTexture(GL_TEXTURE_3D, worleyTex -> getID());
  	glBindImageTexture(0, worleyTex -> getID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glDispatchCompute(INT_CEIL(32, 4), INT_CEIL(32, 4), INT_CEIL(32, 4));
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glCheckError();

	COUT("Computed worely")
	glGenerateMipmap(GL_TEXTURE_3D);


	weatherTex = make_unique<Texture>(1024,1024);
}



void Clouds::generateWeatherMap() {

	glBindImageTexture(0, weatherTex -> getID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	weatherShader -> setVec3("seed", seed);
	weatherShader -> setFloat("perlinFrequency", perlinFrequency);
	
	glDispatchCompute(INT_CEIL(1024, 8), INT_CEIL(1024, 8), 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glCheckError();

	COUT("Computed weather")

	oldSeed = seed;
}


void Clouds::update() {

	if (counter % 300 == 0){
		seed = genRandomVec3();
		generateWeatherMap();
	}
	counter++;
}


void Clouds::initVariables() {

	cloudSpeed = 450.0;
	coverage = 0.15;
	crispiness = 40.;
	curliness = .1;
	density = 0.02;
	absorption = 0.35;

	earthRadius = 600000.0;
	sphereInnerRadius = 5000.0;
	sphereOuterRadius = 17000.0;

	perlinFrequency = .8;

	enableGodRays = false;
	enablePowder = false;
	postProcess = true;

	seed = vec3(0.0, 0.0, 0.0);
	oldSeed = vec3(0.0, 0.0, 0.0);

	cloudColorTop = vec3(169., 149., 149.)*(1.5f / 255.f);
	cloudColorBottom = vec3(65., 70., 80.)*(1.5f / 255.f);
}


void Clouds::draw(GLuint fboTex) {

	static vec3 lightPos = 5.0f * vec3(0,18,18);
	static vec3 lightColor = vec3(0.5,0.0,0.6);
	static vec3 blue = vec3(0.2,0.0,0.2);

	vec3 lightDir = normalize(lightPos - POSITION);

	glUseProgram(cloudsShader -> progID); 
	glBindImageTexture(0, fboTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	//COUT("Binding cloud uniforms")

	cloudsShader -> setVec2("iResolution", vec2(scr_WIDTH, scr_HEIGHT));
	cloudsShader -> setFloat("iTime", CURTIME);
	cloudsShader -> setMat4("inv_proj", inverse(PROJMAT));
	cloudsShader -> setMat4("inv_view", inverse(VIEWMAT));
	cloudsShader -> setVec3("cameraPosition", POSITION);
	cloudsShader -> setFloat("FOV", glm::radians(45.0f));
	cloudsShader -> setVec3("lightDirection", lightDir);
	cloudsShader -> setVec3("lightColor", lightColor);
	
	cloudsShader -> setFloat("coverage_multiplier", coverage);
	cloudsShader -> setFloat("cloudSpeed", cloudSpeed);
	cloudsShader -> setFloat("crispiness", crispiness);
	cloudsShader -> setFloat("curliness", curliness);
	cloudsShader -> setFloat("absorption", absorption*0.01);
	cloudsShader -> setFloat("densityFactor", density);

	//cloudsShader -> setBool("enablePowder", enablePowder);
	
	cloudsShader -> setFloat("earthRadius", earthRadius);
	cloudsShader -> setFloat("sphereInnerRadius", sphereInnerRadius);
	cloudsShader -> setFloat("sphereOuterRadius", sphereOuterRadius);

	cloudsShader -> setVec3("cloudColorTop", cloudColorTop);
	cloudsShader -> setVec3("cloudColorBottom", cloudColorBottom);
	
	cloudsShader -> setVec3("skyColorTop", blue);
	cloudsShader -> setVec3("skyColorBottom", blue);

	cloudsShader -> set3DTexture(perlinTex -> getID(), "perlinTex",  0);
	cloudsShader -> set3DTexture(worleyTex -> getID(), "worleyTex", 1);
	cloudsShader -> set2DTexture(weatherTex -> getID(), "weatherTex", 2);
	//cloudsShader -> set2DTexture(depthTex -> getID(), "depthMap", 3);
	//cloudsShader -> setCubemap(skyTex -> getID(), "sky", 4);

	//COUT("Dispatching compute")

	glDispatchCompute(INT_CEIL(scr_WIDTH, 16), INT_CEIL(scr_HEIGHT, 16), 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	
	//Post effects
	/*
	if (model->postProcess) {
		// cloud post processing filtering
		cloudsPostProcessingFBO->bind();
		Shader& cloudsPPShader = model->postProcessingShader->getShader();

		cloudsPPShader.use();

		cloudsPPShader.setSampler2D("clouds", cloudsFBO->getColorAttachmentTex(VolumetricClouds::fragColor), 0);
		cloudsPPShader.setSampler2D("emissions", cloudsFBO->getColorAttachmentTex(VolumetricClouds::bloom), 1);
		cloudsPPShader.setSampler2D("depthMap", s->sceneFBO->depthTex, 2);

		cloudsPPShader.setVec2("cloudRenderResolution", vec2(scr_WIDTH, scr_HEIGHT));
		cloudsPPShader.setVec2("resolution", vec2(Window::scr_WIDTH , Window::scr_HEIGHT));

		mat4 lightModel;
		lightModel = translate(lightModel, s->lightPos);
		vec4 pos = vp * lightModel * vec4(0.0, 60.0, 0.0, 1.0);
		pos = pos / pos.w;
		pos = pos * 0.5f + 0.5f;

		//std::cout << pos.x << ": X; " << pos.y << " Y;" << std::endl;
		cloudsPPShader.setVec4("lightPos", pos);

		bool isLightInFront = false;
		float lightDotCameraFront = dot(normalize(s->lightPos - s->cam->Position), normalize(s->cam->Front));
		//std::cout << "light dot camera front= " << lightDotCameraFront << std::endl;
		if (lightDotCameraFront > 0.2) {
			isLightInFront = true;
		}

		cloudsPPShader.setBool("isLightInFront", isLightInFront);
		cloudsPPShader.setBool("enableGodRays", model->enableGodRays);
		cloudsPPShader.setFloat("lightDotCameraFront", lightDotCameraFront);

		cloudsPPShader.setFloat("time", glfwGetTime());
		if (!s->wireframe)
			model->postProcessingShader->draw();
	}*/
}