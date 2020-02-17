#include <mesh/Mesh.h>


using namespace glm;
using namespace std;


regex object_header("o ");

regex vertex_decl("v( [-]?[0-9]*\.?[0-9]*){3}");
regex normal_decl("vn( [-]?[0-9]*\.?[0-9]*){3}");
regex face_decl("f( [0-9]*\/\/[0-9]*){3}");

regex float_decl("([-]?[0-9]+\.[0-9]+) ([-]?[0-9]+\.[0-9]+) ([-]?[0-9]+\.[0-9]+)");
regex face_nums("f ([0-9]+)\/\/([0-9]+) ([0-9]+)\/\/([0-9]+) ([0-9]+)\/\/([0-9]+)");

smatch sm;


int read_obj_file(string filename, vector<vec3>& verts, vector<vec3>& normals, vector<TriPrim>& tris){

	std::ifstream file (filename);
	int i = 0;

	std::string::size_type sz;

	float c [3];
	int t [3];
	
	if (!file)
		cout << "unable to open file";
		return 0;
	

	string line;

	while (getline (file, line)) {

		if (regex_search(line, sm, vertex_decl)){
						
			if (regex_search(line, sm, float_decl)) {
			
			    for (int i = 1; i < sm.size(); i++)
			        c[i-1] = stof(sm[i], &sz);

			    verts.push_back(vec3(c[0],c[1],c[2]));
			}
		}
		else if (regex_search(line, sm, normal_decl)){

			if (regex_search(line, sm, float_decl)) {
			
			    for (int i = 1; i < sm.size(); i++)
			        c[i-1] = stof(sm[i], &sz);

			    normals.push_back(vec3(c[0],c[1],c[2]));
			}
		}
		else if (regex_search(line, sm, face_decl)){

			if (regex_search(line, sm, face_nums)) {

			    for (int i = 1; i < sm.size(); i+=2)
			        t[i/2] = stoi(sm[i], &sz);

			    tris.push_back(TriPrim{t[0], t[1], t[2]});
			}
		}
		else {
			//nada
		}
	}
	return 1;
} 
