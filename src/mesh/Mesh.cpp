#include <mesh/Mesh.h>

using namespace glm;
using namespace std;


GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}


int Mesh::draw(){

	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}


int Mesh::deleteBuffers(){

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}


int Mesh::bindBuffers(){

	cout << verts.size() << " " << indices.size() << endl;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), &verts[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    glBindVertexArray(0);
} 




void Mesh::read_obj_file(string filename){

	regex object_header("o ");
	regex vertex_decl("v( [-]?[0-9]*\.?[0-9]*){3}");
	regex normal_decl("vn( [-]?[0-9]*\.?[0-9]*){3}");
	regex face_decl("f( [0-9]*\/\/[0-9]*){3}");
	regex float_decl("([-]?[0-9]+\.[0-9]+) ([-]?[0-9]+\.[0-9]+) ([-]?[0-9]+\.[0-9]+)");
	regex face_nums("f ([0-9]+)\/\/([0-9]+) ([0-9]+)\/\/([0-9]+) ([0-9]+)\/\/([0-9]+)");

	smatch sm;

	ifstream file;
	file.open(filename);

	string::size_type sz;

	float c[3];
	GLuint t[3];
	int i = 0, k = 1;
	
	if (file.fail())
		//file.close();
		cout << "unable to open file: " << filename << endl;
		//cerr << "Error: " << strerror(errno);
		k = 0;

	//if (!k) return;
	
	
	cout << "\nAbout to read " << endl;

	string line;

	while (getline (file, line)) {

		//cout << "online" << endl;

		if (regex_search(line, sm, vertex_decl)){
						
			if (regex_search(line, sm, float_decl)) {
			
			    for (int i = 1; i < sm.size(); i++)
			        c[i-1] = stof(sm[i], &sz);

			    //cout << c[0] << c[1] << c[2] << endl;

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
			        t[i/2] = (GLuint) stoi(sm[i], &sz);

			    //cout << t[0] - 1 << t[1] - 1 << t[2] - 1 << endl;

			    indices.push_back(t[0] - 1);
			    indices.push_back(t[1] - 1);
			    indices.push_back(t[2] - 1);

			    //tris.push_back(TriPrim{t[0], t[1], t[2]});
			}
		}
		else {
			//nada
		}
	}
	file.close();
	cout << "Done reading" << endl;
} 
