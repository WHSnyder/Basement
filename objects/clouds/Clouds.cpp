#include "clouds/Clouds.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <random>


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
		//generateWeatherMap();
	}
	counter++;
}


void Clouds::initVariables() {

	cloudSpeed = 1000.0;
	coverage = 0.55;
	crispiness = 10.;
	curliness = .6;
	density = 0.1;
	absorption = 0.25;

	earthRadius = 600000.0;
	sphereInnerRadius = 5000.0;
	sphereOuterRadius = 17000.0;

	perlinFrequency = 1.8;

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
	static vec3 lightColor = vec3(0.7,0.1,0.1);
	static vec3 blue = vec3(0.2,0.2,0.2);

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

/*
auto input = ctx.graph->FindInputs(ctx.node->id)[0];

    std::vector<Variable> parameters = {
        {"input_data_0_h", input->tensor.shape.h},
        {"input_data_0_w", input->tensor.shape.w},
        {"input_data_0_c", input->tensor.shape.c}};

    std::string source;

    int zDim = ceil(input->tensor.shape.c / 4.0 - .0001);

    std::vector<Variable> shared_variables = {
        {"sh_mem", std::vector<float4>(64 * zDim)},
    };

    if (input->tensor.shape.h * input->tensor.shape.w >= 1024) { //Arbitrary

      source = R"(        
        highp vec4 sum = vec4(0.0);

        float cDim = float($input_data_0_c$);

        highp float inputSize = float($input_data_0_w$ * $input_data_0_h$);

        const int sizeY = int(gl_WorkGroupSize.y);
        const int sizeX = int(gl_WorkGroupSize.x);
        ivec3 localID = ivec3(gl_LocalInvocationID);

        int workGridSize = sizeX * sizeY;
        int taskSize = int(ceil(inputSize/float(workGridSize)));

        int gridID = localID.y * sizeX + localID.x;
        int z_offset = localID.z * int(inputSize);

        int startIndex = gridID * taskSize;

        for (int i = startIndex; i < startIndex + taskSize; i+=1) {
          sum += i < inputSize ? $input_data_0[i + z_offset, 0, 0]$ : vec4(0.0);
        }

        z_offset = localID.z * workGridSize;

        sh_mem[gridID + z_offset] = sum;

        memoryBarrierShared();
        barrier();

        if (gid.x >= 1 || gid.y >= 1){
          return;
        }

        sum = vec4(0.0);
        
        for (int i = 0; i < workGridSize; i++){
          sum += sh_mem[i + z_offset];  
        }

        value_0 = sum / inputSize;
      )";
	*/
      //*generated_code = {
          /*parameters=*///std::move(parameters),
          /*objects=*///{},
          /*shared_variables=*///std::move(shared_variables),
          /*workload=*///uint3(1,1,1),
          /*workgroup=*///uint3(8,8,zDim), //Someone more knowledgeable should change this,
                                         //I kept it small for compatibility/simplicity.
          /*source_code=*///std::move(source),
          /*input=*///IOStructure::ONLY_DEFINITIONS,
          /*output=*///IOStructure::AUTO,
      //};
    //}




//WITH GLOBAL SCOPE
/*
constexpr int kWorkgroupHintX = 4;
    constexpr int kWorkgroupHintY = 4;
    int kWorkgroupHintZ = ceil(input->tensor.shape.c / 4.0 - .0001);

    std::vector<Variable> shared_variables = {
        {"sh_mem", std::vector<float4>(kWorkgroupHintX/2 * kWorkgroupHintX * 
                                       kWorkgroupHintY/2 * kWorkgroupHintY * 
                                       kWorkgroupHintZ)},
    };


    if (input->tensor.shape.h * input->tensor.shape.w >= 1024) {

      source = R"(        
        highp vec4 sum = vec4(0.0);
        highp float size = float($input_data_0_w$ * $input_data_0_h$);

        const int groupsX = int(gl_NumWorkGroups.x);
        const int groupsY = int(gl_NumWorkGroups.y);
        const int sizeX = int(gl_WorkGroupSize.x);
        const int sizeY = int(gl_WorkGroupSize.y);
        ivec3 localID = ivec3(gl_LocalInvocationID.xyz);
        int workGridSize = sizeX * sizeY * groupsX * groupsY;

        int local_grid_index = sizeX * localID.y + localID.x;
        int global_grid_index = gid.x * sizeX * sizeY * groupsY + gid.y * sizeX * sizeY;
        
        int taskSize = int(ceil(size / float(workGridSize)));

        int flattenedIndex = global_grid_index + local_grid_index;
        int z_offset = gid.z * int(size);

        int startIndex = flattenedIndex * taskSize;

        for (int i = startIndex; i < startIndex + taskSize; i++) {
          sum += i < size ? $input_data_0[z_offset + i, 0, 0]$ : vec4(0.0);
        }

        z_offset = gid.z * workGridSize;

        sh_mem[flattenedIndex + z_offset] = sum;

        memoryBarrierShared();
        groupMemoryBarrier();
        barrier();

        if (gid.x >= 1 || gid.y >= 1){
          return;
        }

        sum = vec4(0.0);
        
        for (int i = 0; i < workGridSize; i++){
          sum += sh_mem[i + z_offset];  
        }

        value_0 = sum / size;
      )";*/

      //*generated_code = {
          /*parameters=*///std::move(parameters),
          /*objects=*///{},
          /*shared_variables=*///std::move(shared_variables),
          /*workload=*///uint3(kWorkgroupHintX,kWorkgroupHintY,1),
          /*workgroup=*/ //uint3(kWorkgroupHintX/2,kWorkgroupHintY/2,kWorkgroupHintZ), 
          /*source_code=*///std::move(source),
          /*input=*///IOStructure::ONLY_DEFINITIONS,
          /*output=*///IOStructure::AUTO,
      //};
    //}