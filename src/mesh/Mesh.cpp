#include <mesh/Mesh.h>


using namespace glm;
using namespace std;


int Mesh::draw(){



	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



int Mesh::bindBuffers(){

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), &verts[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    // vertex normals
    /*
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    */

    glBindVertexArray(0);
} 




int read_obj_file(string filename, vector<vec3>& verts, vector<vec3>& normals, vector<int>& inds){

	regex object_header("o ");

	regex vertex_decl("v( [-]?[0-9]*\.?[0-9]*){3}");
	regex normal_decl("vn( [-]?[0-9]*\.?[0-9]*){3}");
	regex face_decl("f( [0-9]*\/\/[0-9]*){3}");

	regex float_decl("([-]?[0-9]+\.[0-9]+) ([-]?[0-9]+\.[0-9]+) ([-]?[0-9]+\.[0-9]+)");
	regex face_nums("f ([0-9]+)\/\/([0-9]+) ([0-9]+)\/\/([0-9]+) ([0-9]+)\/\/([0-9]+)");

	smatch sm;


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

			    inds.push_back(t[0]);
			    inds.push_back(t[1]);
			    inds.push_back(t[2]);

			    //tris.push_back(TriPrim{t[0], t[1], t[2]});
			}
		}
		else {
			//nada
		}
	}

	//maybe add more stuff herefor mesh verts

	return 1;
} 
