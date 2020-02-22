#include <stdlib.h> 

#include <vector>

#include <string>

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <ext.hpp>

#include <GL/glew.h>


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
		vector<MeshVertex> verts;

		GLuint VAO, VBO, EBO;

		/*Mesh(vector<GLuint> inds, vector<vec3> vertices, vector<vec3> normz){
			indices = inds;
			verts = vertices;
			normals = normz;
		}*/

		Mesh(string filename){
			read_obj_file(filename);
		}
};