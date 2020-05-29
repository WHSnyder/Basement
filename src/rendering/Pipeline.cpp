#include <stack>
#include "RenderGraphNodeDefs.h"
#include <GL/gl3w.h>    
//#include <GLFW/glfw3.h>

//Implementation for the OpenGL backend of the nodes system.

using namespace std;


class Pool {

	vector<Mesh *> meshes;
	vector<mat4> modelMats;
	static int poolCounter = 0;
	int poolID;

	Pool(){
		poolID = poolCounter++;
	}

	void addMat(mat4 modelMat){
		modelMats.push_back(modelMat);
	}

	void addMesh(Mesh *mesh){
		meshes.push_back(mesh);
	}

	int getSize(){
		return modelMats.size();
	}
}

//For now, pools and textures have no inputs, thus process returns immediately.
void PoolNode::process(){ return; }
void TextureNode::process(){ return; }

void FBONode::process(){

	if (visited) return;

	//Process all inputs 	
	for (auto &&pin : inputs)
		pin -> node -> process();

	visited = 1;
}

void FBONode::init(){

	if (outputs[0] -> node -> Type == NodeType::	)
}

void ShaderNode::process(){

	//For now, we simply render to all FBOs the shader outputs to, so we dont have to revisit the shader later 
	if (visited) return;

	//Since FBOs are dynamic, we must loop through them and fill them before proceeding
	for (auto &&pin : inputs)
		pin -> node -> process();
	
	//for each output fbo
		//for each pool
			//bind vaos, uniforms
			//render

	//For now, render to all FBOs connected.  Is not in the spirit of intuitive DFS, perhaps per FBO approach is better?
	//This is simpler so will be used for MVP
	//Still need to handle post processes
	for (auto &&pin : outputs){

		Node *curNode = pin -> node;
		FBONode *fboOutput = reinterpret_cast<FBONode *> curNode;

		fboOutput -> target -> set();

		for (auto &&poolPin : inputs){

			Node *curNodePool = poolPin -> node;

			if (curNodePool -> Type != NodeType::Pool) continue;

			PoolNode *poolNode = reinterpret_cast<PoolNode *> curNodePool; //Slow but good for now
			Pool *curPool = poolNode -> pool;

			for (int i=0; i < curPool -> getSize(); i++){

				//A ton of redundant GL calls here, will reorg interface soon. 
				shaderObj -> setModel(value_ptr(curPool -> modelMats[i]));
				curPool -> meshes[i] -> draw(shaderObj -> progID);
			}
		}	
	}	

	//Shader has read its inputs and written to all targets.
	visited = 1;
}

void ShaderNode::init(){

	for (auto &&pin : inputs){

		Node *curNode = pin -> node;
		NodeType type = curNode -> Type;

		if (type == NodeType::Pool) continue;

		if (type == NodeType:: )


	}	
}



//Handles all static bindings, runs at beginning of game.
//Binds textures to shaders, for now uses global active tex IDs, will have to change.
/*
void bindStaticsAwake(vector<Node *> nodes){
	
	for (auto Node&& node : nodes){

		if (!node -> static_flag) node -> visited = 0;



	}

	//for all inputs of finalfbo
		//
}*/