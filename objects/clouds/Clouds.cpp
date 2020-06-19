#include "Clouds.h"

#define TIMETO(CODE, TASK) 	t1 = glfwGetTime(); CODE; t2 = glfwGetTime(); std::cout << "Time to " + std::string(TASK) + " :" << (t2 - t1)*1e3 << "ms" << std::endl;


extern glm::vec3 POSITION;
extern int SCR_WIDTH, SCR_HEIGHT;


using namespace glm;


CloudsModel::CloudsModel(sceneElements * scene, Skybox * sky) : scene(scene), sky(sky){
	initVariables();
	initShaders();
	generateModelTextures();
}


void CloudsModel::initShaders(){
	cloudsShader = make_unique<Shader>("assets/shaders/volumetric_clouds",1);
	
	//postProcessingShader = new ScreenSpaceShader("shaders/clouds_post.frag");
	//compute shaders
	//weatherShader = new Shader("weatherMap");
	//weatherShader->attachShader("shaders/weather.comp");
	//weatherShader->linkPrograms();
}


void CloudsModel::generateModelTextures() {
				
	perlinWorelyShader = make_unique<Shader>("assets/shaders/perlin_worley",1);
	perlinTex = make_unique<Texture>(128, 128, 128);
	
	//perlinWorelyShader -> setVec3("u_resolution", vec3(128, 128, 128));
	//perlinWorelyShader -> setInt("outVolTex", 0);

	glUseProgram(perlinWorelyShader -> progID); //Need to abstract that...
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, perlinTex -> getID());
  	glBindImageTexture(0, perlinTex -> getID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glDispatchCompute(INT_CEIL(128, 4), INT_CEIL(128, 4), INT_CEIL(128, 4));
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
	COUT("Computed worely")
	glGenerateMipmap(GL_TEXTURE_3D);

	
	worleyShader = make_unique<Shader>("assets/shaders/worley", 1);
	worleyTex = make_unique<Texture>(32, 32, 32);
	
	glUseProgram(worleyShader -> progID);
	glActiveTexture(GL_TEXTURE0);
  	glBindTexture(GL_TEXTURE_3D, worleyTex -> getID());
  	glBindImageTexture(0, worleyTex -> getID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glDispatchCompute(INT_CEIL(32, 4), INT_CEIL(32, 4), INT_CEIL(32, 4));
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	COUT("Wrote worely noise")
	glGenerateMipmap(GL_TEXTURE_3D);


	if (1) {
		//make texture
		//this->weatherTex = generateTexture2D(1024, 1024);

		//compute
		//generateWeatherMap();

		//seed = scene->seed;
		//oldSeed = seed;
	}
}


void CloudsModel::update(){

	seed = scene->seed;
	if (seed != oldSeed) {
		generateWeatherMap();
		oldSeed = seed;
	}
}


void CloudsModel::initVariables(){

	cloudSpeed = 450.0;
	coverage = 0.45;
	crispiness = 40.;
	curliness = .1;
	density = 0.02;
	absorption = 0.35;

	earthRadius = 600000.0;
	sphereInnerRadius = 5000.0;
	sphereOuterRadius = 17000.0;

	perlinFrequency = 0.8;

	enableGodRays = false;
	enablePowder = false;
	postProcess = true;

	seed = vec3(0.0, 0.0, 0.0);
	oldSeed = vec3(0.0, 0.0, 0.0);

	cloudColorTop = vec3(169., 149., 149.)*(1.5f / 255.f);
	cloudColorBottom = vec3(65., 70., 80.)*(1.5f / 255.f);
}




void Clouds::draw(GLuint depthTex) {

	float t1, t2;

	//cloudsFBO->bind();
	for (int i = 0; i < cloudsFBO->getNTextures(); ++i) 
		bindTexture2D(cloudsFBO->getColorAttachmentTex(i), i);
	

	sceneElements* s = drawableObject::scene;

	cloudsShader -> use();

	cloudsShader -> setVec2("iResolution", vec2(SCR_WIDTH, SCR_HEIGHT));
	cloudsShader -> setFloat("iTime", glfwGetTime());
	cloudsShader -> setMat4("inv_proj", inverse(PROJMAT));
	cloudsShader -> setMat4("inv_view", VIEWMAT);
	cloudsShader -> setVec3("cameraPosition", POSITION);
	cloudsShader -> setFloat("FOV", s->cam->Zoom);
	cloudsShader -> setVec3("lightDirection", normalize(s->lightPos - s->cam->Position));
	cloudsShader -> setVec3("lightColor", s->lightColor);
	
	cloudsShader -> setFloat("coverage_multiplier", model->coverage);
	cloudsShader -> setFloat("cloudSpeed", model->cloudSpeed);
	cloudsShader -> setFloat("crispiness", model->crispiness);
	cloudsShader -> setFloat("curliness", model->curliness);
	cloudsShader -> setFloat("absorption", model->absorption*0.01);
	cloudsShader -> setFloat("densityFactor", model->density);

	//cloudsShader -> setBool("enablePowder", enablePowder);
	
	cloudsShader -> setFloat("earthRadius", model->earthRadius);
	cloudsShader -> setFloat("sphereInnerRadius", model->sphereInnerRadius);
	cloudsShader -> setFloat("sphereOuterRadius", model->sphereOuterRadius);

	cloudsShader -> setVec3("cloudColorTop", model->cloudColorTop);
	cloudsShader -> setVec3("cloudColorBottom", model->cloudColorBottom);
	
	cloudsShader -> setVec3("skyColorTop", model->sky->skyColorTop);
	cloudsShader -> setVec3("skyColorBottom", model->sky->skyColorBottom);

	//mat4 vp = projMat * viewMat;
	//cloudsShader -> setMat4("inv_view", inverse(vp));
	//cloudsShader -> setMat4("gVP", vp);

	cloudsShader -> setTexture(perlinTex -> getID(), "cloud",  0);
	cloudsShader -> setTexture(worley -> getID(), "worley32", 1);
	cloudsShader -> setTexture(depthTex, "depthMap", 3);
	
	//cloudsShader -> setTexture("weatherTex", model->weatherTex, 2);
	//cloudsShader -> setTexture(model->sky->getSkyTexture(), "sky", 4);

	glDispatchCompute(INT_CEIL(SCR_WIDTH, 16), INT_CEIL(SCR_HEIGHT, 16), 1);
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

		cloudsPPShader.setVec2("cloudRenderResolution", vec2(SCR_WIDTH, SCR_HEIGHT));
		cloudsPPShader.setVec2("resolution", vec2(Window::SCR_WIDTH , Window::SCR_HEIGHT));

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




/*
VolumetricClouds::VolumetricClouds(int SW, int SH, CloudsModel * model): SCR_WIDTH(SW), SCR_HEIGHT(SH), model(model) {

	cloudsFBO = new TextureSet(SW, SH, 4);
	cloudsPostProcessingFBO = new FrameBufferObject(Window::SCR_WIDTH, Window::SCR_HEIGHT, 2);
}
*/




/*
void CloudsModel::generateWeatherMap() {
	bindTexture2D(weatherTex, 0);
	weatherShader->use();
	weatherShader->setVec3("seed", scene->seed);
	weatherShader->setFloat("perlinFrequency", perlinFrequency);
	//COUT("computing weather!")
	glDispatchCompute(INT_CEIL(1024, 8), INT_CEIL(1024, 8), 1);
	//COUT("weather computed!!")

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}*/