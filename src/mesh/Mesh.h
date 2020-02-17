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


struct TriPrim { int a,b,c; };

struct MeshVertex {
	vec3 position, normal;
};


int read_obj_file(string filename, vector<vec3>& verts, vector<vec3>& normals, vector<int>& tris);


class Mesh {

	public:

		vector<int> indices;
		vector<vec3> verts, normals;
		vector<MeshVertex> mesh_verts;


		//VAO = vertex attribute object  
		//VBO = vertex buffer object
		//EBO = element (index) buffer object
		unsigned int VAO, VBO, EBO;

		Mesh(vector<int> inds, vector<vec3> vertices, vector<vec3> normz){
			indices = inds;
			verts = vertices;
			normals = normz;
		}

		Mesh(string filename){
			read_obj_file(filename, verts, normals, indices);
		}

		int bindBuffers();
		int draw();

};