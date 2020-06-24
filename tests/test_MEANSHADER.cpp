#include "GameContext.h"

#include "rendering/Texture.h"
#include "rendering/Shader.h"




Shader *meanShader;
int bufferSize = 256 * 256 * 4;
float *dataOutSSBO, *dataInSSBO;
std::vector<float> outVector(16);


int main(int argc, char **argv){

	initialize_window();

	meanShader = new Shader("assets/shaders/mean_shader_test", 1);

	dataOutSSBO = calloc(512 * 4, sizeof(float));

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

	delete meanShader;
	delete dataOutSSBO;
	delete dataInSSBO;

	destroy_context();
}