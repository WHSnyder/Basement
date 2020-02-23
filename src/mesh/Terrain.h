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


class Terrain {

	public:

		int bindBuffers();
		int deleteBuffers();
		int draw();
		void read_obj_file(string filename);
		void generate_terrain(int width, int height, int numw, int numh);

		vector<GLuint> indices;
		vector<MeshVertex> verts;

		GLuint VAO, VBO, EBO;

		Mesh(string filename){
			read_obj_file(filename);
		}
};
