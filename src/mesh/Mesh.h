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


using namespace glm;
using namespace std;



struct TriPrim { int a,b,c; };


int read_obj_file(string filename, vector<vec3>& verts, vector<vec3>& normals, vector<TriPrim>& tris);

class Mesh {

	public:

		vector<TriPrim> tris;
		vector<vec3> verts, normals;

		Mesh(vector<TriPrim> triangles, vector<vec3> vertices, vector<vec3> normz){
			tris = triangles;
			verts = vertices;
			normals = normz;
		}

		Mesh(string filename){
			read_obj_file(filename, verts, normals, tris);
		}
};
