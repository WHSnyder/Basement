#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>


class Node {


};


class ShaderNode : Node {

public:
	Shader shader;
	ShaderNode(Shader shader_pbj);
};


class FBONode : Node {

public:
	RenderTarget targ;
	FBONode(RenderTarget rend_targ);
};




class Pipeline {

	public:

		Material *mat;
		Mesh *mesh;

		

		Pipeline();

}



