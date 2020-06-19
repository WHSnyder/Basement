#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rendering/Texture.h"
#include "rendering/Shader.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>


#define INT_CEIL(n,d) (int)ceil((float)n/d)
#define COUT(x) std::cout << x << std::endl;


//CloudsModel is responsible to collect the attributes and shaders that will be needed to render the volumetric clouds. Also, it creates the noises which models the clouds.
class Clouds {

public:
	
	Clouds(int w, int h);
	//~Clouds();

	void update();
	void draw(unsigned int fboTex);

	std::unique_ptr<Shader> cloudsShader, perlinWorelyShader, worleyShader, weatherShader;
	//ScreenSpaceShader *postProcessingShader;

	float coverage, cloudSpeed, crispiness, curliness, density, absorption;
	float earthRadius, sphereInnerRadius, sphereOuterRadius;
	float perlinFrequency;
	bool enableGodRays, enablePowder, postProcess;

	glm::vec3 cloudColorTop, cloudColorBottom;
	glm::vec3 seed, oldSeed;
	std::unique_ptr<Texture> perlinTex, worleyTex, weatherTex;

	//sceneElements *scene;
	//Skybox *sky;

private:

	void generateWeatherMap();
	void generateModelTextures();
	void initVariables();
	void initShaders();
	//void generateWeatherMap();

	enum cloudsTextureNames {fragColor, bloom, alphaness, cloudDistance};

	//TextureSet *cloudsFBO;
	//FrameBufferObject *cloudsPostProcessingFBO;
	//CloudsModel *model;
};
