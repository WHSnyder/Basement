#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include <mesh/Mesh.h>
#include <phys/Physics.h>

#include <utils/ShaderUtils.h>

#include <gtx/transform.hpp>

#include <perlin/PerlinNoise.hpp>


using namespace std;

void coutMat(float *mat){
	cout << "{" << mat[0] << ", " << mat[1] << ", " << mat[2] << ", " << mat[3] << "}" << endl;
	cout << " " << mat[4] << ", " << mat[5] << ", " << mat[6] << ", " << mat[7] << "," << endl;
	cout << " " << mat[8] << ", " << mat[9] << ", " << mat[10] << ", " << mat[11] << "," << endl;
	cout << " " << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << "}" << endl;
}

float *generate_terrain(int cols, int rows){

	cout << "Generating terrain" << endl;

	//vertices.push_back(vec3(0,0,0));
	//MeshVertex mv;

	std::uint32_t seed = 12345;

	const siv::PerlinNoise perlin(seed);
	float *result = (float *) calloc(cols * rows, sizeof(float));
	double fx = cols/8.0, fy = rows/8.0;

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){

			float per = (float) perlin.accumulatedOctaveNoise2D_0_1(j/fx, i/fy, 8);

			if (j % 10 == 0){
				cout << per << endl;
			}

			/*mv.uv.x = j;
			mv.uv.y = i;

			mv.position.x = 
			verts.push_back(mv);

			if (i < rows - 1 || j < cols - 1){

				//1st tri
				indices.push_back(cols * i + j);
				indices.push_back(cols * i + j + 1);
				indices.push_back(cols * (i+1) + j);

				//2nd tri
				indices.push_back(cols * i + j + 1);
				indices.push_back(cols * (i+1) + j + 1);
				indices.push_back(cols * i + j);
			}*/

			result[i * cols + j] = per;
		}
	}

	return result;
}


static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int compile_shader(GLenum shaderType, string shaderCode){
	
	unsigned int shader;
	int success;
	char infoLog[512];
	const char *contents = shaderCode.c_str();
	   
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &contents, NULL);
	glCompileShader(shader);
	//glCheckError();

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(!success){
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	  
	return shader;
}



Mesh *gen_plane(){

	vec3 verts[] = {vec3(-1,1,1), vec3(1,1,1), vec3(-1,-1,1), vec3(1,-1,1)};
	vec3 norms[] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1)};
	GLuint inds[] = {0,1,2,1,3,2};

	return new Mesh(vector<vec3>(verts,verts+4), vector<vec3>(norms,norms+4), vector<GLuint>(inds,inds+6));
}


GLuint bindTexture(int cols, int rows, float *data){

	GLuint tex;
	//glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glCheckError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glCheckError();

    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                GL_RED,            // Internal colour format to convert to
                cols,          // Image width  i.e. 640 for Kinect in standard mode
                rows,          // Image height i.e. 480 for Kinect in standard mode
                0,                 // Border width in pixels (can either be 1 or 0)
                GL_RED, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                GL_FLOAT,  // Image data type
                (void *) data);        // The actual image data itself

    glGenerateMipmap(GL_TEXTURE_2D);
    glCheckError();

    return tex;
}


int main(int argc, char **argv){
	
	GLFWwindow* window;

	Simu mainSimu;

	string path("/Users/will/projects/cpprtx/meshes/cube.obj");
	Mesh *cube = new Mesh(path);

	string path1("/Users/will/projects/cpprtx/meshes/ball.obj");
	Mesh *sphere = new Mesh(path1);

	Mesh *plane = gen_plane();
	
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
	window = glfwCreateWindow(480, 480, "Test", NULL, NULL);
	if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

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

	//Binding phys test objs
   	cube -> bindBuffers();
   	sphere -> bindBuffers();

    string vshader = read_shader("src/rendering/shaders/BasicVert.hlsl");
    string fshader = read_shader("src/rendering/shaders/BasicFrag.hlsl");

    unsigned int vertex = compile_shader(GL_VERTEX_SHADER, vshader);
    unsigned int fragment = compile_shader(GL_FRAGMENT_SHADER, fshader);
    glCheckError();

    unsigned int ID = glCreateProgram();
    int success;

    char infoLog[512];

    glCheckError();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glCheckError();

	glLinkProgram(ID);
	glCheckError();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glCheckError();


    //Binding noise test objs
    int rows = 30, cols = 30;
    float *img_data = generate_terrain(cols,rows);

    GLuint tex_num = bindTexture(cols,rows,img_data);
    glCheckError();

   	plane -> bindBuffers();

    string noise_vshader = read_shader("src/rendering/shaders/NoiseTestVert.glsl");
    string noise_fshader = read_shader("src/rendering/shaders/NoiseTestFrag.glsl");

    unsigned int n_vertex = compile_shader(GL_VERTEX_SHADER, noise_vshader);
    unsigned int n_fragment = compile_shader(GL_FRAGMENT_SHADER, noise_fshader);

    unsigned int n_ID = glCreateProgram();

    glCheckError();
	glAttachShader(n_ID, n_vertex);
	glAttachShader(n_ID, n_fragment);
	glCheckError();

	glLinkProgram(n_ID);
	glCheckError();

	glUseProgram(n_ID);

	GLint n_projloc = glGetUniformLocation(n_ID, "p");
    GLint n_lookloc = glGetUniformLocation(n_ID, "v");
    GLint n_texloc = glGetUniformLocation(n_ID, "tex");

    glUniform1i(n_texloc,0);

    glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, tex_num);






    auto t_start = std::chrono::high_resolution_clock::now();
	auto t_now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

	mat4 proj = perspective(glm::radians(45.0f), 1.0f, 0.01f, 100.0f);
	mat4 rot = mat4(1.0),testmat;
	mat4 dest = mat4(1.0);
	mat4 trans = transpose(translate(vec3(-1,1.0,-20.0)));
	mat4 trans2 = transpose(translate(vec3(1,1.0,-20.0)));

	mat4 view = lookAt(vec3(0,0,5),vec3(0,0,-100),vec3(0,1.0,0));
    
    /*GLint projloc = glGetUniformLocation(ID, "p");
    GLint rotloc = glGetUniformLocation(ID, "m");
    GLint lookloc = glGetUniformLocation(ID, "v");

	glUniformMatrix4fv(projloc, 1, GL_FALSE, value_ptr(proj));
	glUniformMatrix4fv(lookloc, 1, GL_FALSE, value_ptr((view)));

	glCheckError();
	*/

	float sphereMat[16] = {}, boxMat[16] = {};

	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		t_start = t_now;

		mainSimu.stepSimu(time);
		mainSimu.getModelMats(sphereMat, boxMat);

		//cout << "=========================" << endl;
		//coutMat((boxMat));
		//cout << "-------------------------" << endl;
		//coutMat(value_ptr(trans));
		//cout << "-------------------------" << endl;
		//testmat = trans * rot;
		//coutMat(value_ptr(testmat));
		//cout << "=========================" << endl;

		glUseProgram(n_ID);
		glUniformMatrix4fv(n_projloc, 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(n_lookloc, 1, GL_FALSE, value_ptr(view));

		plane -> draw();


		//rot = rotate(rot, time * glm::radians(20.0f), vec3(0.0f,1.0f,0.0f));

		//glUniformMatrix4fv(rotloc, (GLuint) 1, GL_FALSE, boxMat);// value_ptr(transpose(dest)));

		//glCheckError();
		//cube -> draw();

		//memcpy(value_ptr(dest),sphereMat,16 * sizeof(float));

		//glUniformMatrix4fv(rotloc, (GLuint) 1, GL_FALSE, sphereMat);//value_ptr(transpose(dest)));
		//glCheckError();
		
		//sphere -> draw();



		glfwSwapBuffers(window);
		glfwPollEvents();
	} 

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	cube -> deleteBuffers();
	delete cube;
	sphere -> deleteBuffers();
	delete sphere;
	plane -> deleteBuffers();
	delete plane;

	delete img_data;

	glDeleteProgram(ID);
	glfwTerminate();

	return 0;
}