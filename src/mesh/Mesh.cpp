#include <mesh/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace glm;
using namespace std;

extern GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

extern string basepath;


int Mesh::draw(GLuint id){

    glUseProgram(id);

	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    glCheckError();
}


int Mesh::deleteBuffers(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}


int Mesh::bindBuffers(){

	cout << "Num vertices: " << verts.size() << " Num triangles: " << indices.size() / 3 << endl;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(MeshVertex), &verts[0], GL_STATIC_DRAW);  

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) 0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) offsetof(MeshVertex,normal));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
} 



void Mesh::read_obj_file(string filename){

	cout << "Loading mesh at " << filename << endl;

    filename.insert(0, basepath);

	Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

    //Check errors
    if ((!scene) || (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)){
        cerr << "Error loading w/assimp" << string(importer.GetErrorString()) << endl;
        //Return fail
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i){

        aiMesh* mesh = scene -> mMeshes[i];
        MeshVertex mv = MeshVertex();
  
        //Iterate over the vertices of the mesh
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j){

            //Set positions
            mv.position.x = mesh -> mVertices[j].x;
            mv.position.y = mesh -> mVertices[j].y;
            mv.position.z = mesh -> mVertices[j].z;

            //Set normals
            mv.normal.x = mesh -> mNormals[j].x;
            mv.normal.y = mesh -> mNormals[j].y;
            mv.normal.z = mesh -> mNormals[j].z;

            //Add vertex
            verts.push_back(mv);
        }

        //Iterate over faces
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j){
            
            //Get face
            aiFace face = mesh->mFaces[j];
            
            //Add indices
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {indices.push_back(face.mIndices[k]);}
        }
    }
}


#ifdef PYBIND
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(Mesh, m) {
    py::class_<Texture>(m, "Texture")
        .def(py::init<std::string>())
        .def("draw", &Mesh::draw);
}
#endif