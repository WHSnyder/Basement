#include <cstdlib> 
#include <cstdio>

#include <vector>

#include <regex> 
#include <string>

#include <fstream>
#include <iostream>
#include <string>

#include <glm.hpp>
#include <ext.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


using namespace glm;
using namespace std;



struct MeshVertex {
	vec3 position, normal;
};


class Mesh {

	public:

		int bindBuffers();
		int draw();
		void read_obj_file(string filename);

		vector<unsigned int> indices;
		vector<vec3> verts, normals;

		//VAO = vertex attribute object  
		//VBO = vertex buffer object
		//EBO = element (index) buffer object
		unsigned int VAO, VBO, EBO;

		Mesh(vector<unsigned int> inds, vector<vec3> vertices, vector<vec3> normz){
			indices = inds;
			verts = vertices;
			normals = normz;
		}

		Mesh(string filename){
			cout << "reading" << endl;
			read_obj_file(filename);
		}
};