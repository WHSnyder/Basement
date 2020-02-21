#include <mesh/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>

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
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(MeshVertex), &verts[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) 0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), ((void*) offsetof(MeshVertex,normal)));

    glBindVertexArray(0);
} 




void Mesh::read_obj_file(string filename){

	cout << "Starting assimp load" << endl;

	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_GenNormals);

    //Check for errors
    if ((!scene) || (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)){
        cerr << "Error loading w/assimp" << string(importer.GetErrorString()) << endl;
        //Return fail
        return -1;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i){

        aiMesh* mesh = scene->mMeshes[i];
        MeshVertex mv = MeshVertex();
  
        //Iterate over the vertices of the mesh
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j){

            //Set the positions
            mv.position.x = mesh->mVertices[j].x;
            mv.position.y = mesh->mVertices[j].y;
            mv.position.z = mesh->mVertices[j].z;

            //Set the normals
            mv.normal.x = mesh->mNormals[j].x;
            mv.normal.y = mesh->mNormals[j].y;
            mv.normal.z = mesh->mNormals[j].z;

            //Add the vertex to the vertices vector
            verts.push_back(mv);
        }

        //Iterate over the faces of the mesh
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j){
            //Get the face
            aiFace face = mesh->mFaces[j];
            //Add the indices of the face to the vector
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {indices.push_back(face.mIndices[k]);}
        }
    }




	/*regex object_header("o ");
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
	GLuint t[6];
	int i = 0, k = 1, dataflag = 0;
	
	if (file.fail())
		//file.close();
		cout << "unable to open file: " << filename << endl;
		//cerr << "Error: " << strerror(errno);
		k = 0;	

	string line;
	getline (file, line)

	while (getline (file, line)) {

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

			int numverts = verts.size();
			uint *bitmap = new uint[numverts]();

			do {
				if (regex_search(line, sm, face_nums)) {

				    for (int i = 1; i < sm.size(); i++)
				        t[i-1] = (GLuint) stoi(sm[i], &sz);

				    for (i = 0; i < 6; i+=2){
				    	
				    	if ((bitmap[i] | (1 << t[i+1])) - bitmap[i] != 0) {
				    		
				    		bitmap[i] |= 1 << t[i+1];

				    		vertex_structs.push_back(MeshVertex(verts[i], normals[i+1]));

				    	}
				    	else struct_indices.push_back(bitmap[i + numverts]);
				    }
				}
			}  while (getline (file, line));

			return 
		}
		else {
			//nada
		}
	}



	file.close();
	cout << "Done reading" << endl;*/
} 
