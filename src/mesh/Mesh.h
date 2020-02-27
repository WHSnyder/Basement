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
	glm::vec2 uv;
};


class Mesh {

	public:

		int bindBuffers();
		int deleteBuffers();
		int draw(GLuint id);
		void read_obj_file(string filename);
		void generate_terrain(int width, int height, int numw, int numh);

		vector<GLuint> indices;
		vector<MeshVertex> verts;

		GLuint VAO, VBO, EBO;

		Mesh(string filename){
			read_obj_file(filename);
			bindBuffers();
		}

		Mesh(vector<vec3> vertices, vector<vec3> norms, vector<GLuint> inds){
			
			indices = inds;
			MeshVertex mv;

			for (int i = 0; i < vertices.size(); i++){

				mv.position = vertices[i];
				mv.normal = norms[i];

				verts.push_back(mv);
			}

			bindBuffers();
		}

		~Mesh(){
			deleteBuffers();
		}
};