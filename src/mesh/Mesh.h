#include <stdlib.h> 
#include <stdio.h>

#include <vector>

#include <regex> 
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <ext.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace glm;
using namespace std;


GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 



struct MeshVertex {
	glm::vec3 position, normal;
};


class Mesh {



	public:

		int bindBuffers();
		int deleteBuffers();
		int draw();
		void read_obj_file(string filename);

		vector<GLuint> indices;
		vector<vec3> verts, normals;

		//VAO = vertex attribute object  
		//VBO = vertex buffer object
		//EBO = element (index) buffer object
		GLuint VAO, VBO, EBO;

		Mesh(vector<GLuint> inds, vector<vec3> vertices, vector<vec3> normz){
			indices = inds;
			verts = vertices;
			normals = normz;
		}

		Mesh(string filename){
			cout << "reading" << endl;
			read_obj_file(filename);
		}
};