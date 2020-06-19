#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <camera.h>
#include <algorithm>

#include "CloudsModel.h"

//VolumetricClouds handles the FrameBufferObjects (and textures) where the clouds will be rendered, and it's responsible to set up the uniforms and calling the draw command.
class VolumetricClouds {

public:
	VolumetricClouds(int SW, int SH, CloudsModel *model);
	virtual void draw();
	~VolumetricClouds();

	enum cloudsTextureNames {fragColor, bloom, alphaness, cloudDistance};

	void generateWeatherMap();

	GLuint getCloudsTexture() { 
		return (model->postProcess ? cloudsPostProcessingFBO->getColorAttachmentTex(0) : getCloudsRawTexture());
	}

	GLuint getCloudsTexture(int i) {
		return cloudsFBO->getColorAttachmentTex(i);
	}

	GLuint getCloudsRawTexture(){
		return cloudsFBO->getColorAttachmentTex(0);
	}

private:
	int SCR_WIDTH, SCR_HEIGHT;

	TextureSet *cloudsFBO;
	FrameBufferObject *cloudsPostProcessingFBO;
	CloudsModel *model;
};