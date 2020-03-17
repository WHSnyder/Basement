#include <stack>

//Implementation for the OpenGL backend of the nodes system.

using namespace std;


struct Pin{
    ed::PinId   ID;
    ::Node*     Node;
    std::string Name;
    PinType     Type;
    PinKind     Kind;
    Pin(int id, const char* name, PinType type):
        ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
    {}
};
struct Node {
    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs, Outputs;
    void *objptr;
    int num;
    ImColor Color;
    NodeType Type;
    ImVec2 Size;
    void *data;
    int static_flag;
    std::string State, SavedState;
    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255), NodeType type = NodeType::Blueprint):
        ID(id), Name(name), Color(color), Type(type), Size(0, 0), num(type == NodeType::Texture ? ++texture_cnt : 0)
    {}
};




class Pool {

	vector<Mesh *> meshes;
	vector<mat4> modelMats;

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





void ShaderNode::process(){

	//For now, we simply render to all FBOs the shader outputs to, so we dont have to revisit the shader later 
	if (visited) return;

	//Since FBOs are dynamic, we must loop through them and fill them before proceeding
	for (auto &&pin : inputs){

		Node *curNode = pin -> node;

		//Will return instantly if node is static (textures, pools, NOT FBOs)
		curNode -> process();
	}
	
	//set current program
	glUseProgram(shaderObj -> progID);
		
	//for each output fbo
		//for each pool
			//bind vaos, uniforms
			//render

	//For now, render to all FBOs connected.  Is not in the spirit of intuitive DFS, perhaps per FBO approach is better?
	//This is simpler so will be used for MVP
	for (auto &&pin : outputs){

		Node *curNode = pin -> node;
		FBONode *fboOutput = reinterpret_cast<FBONode *> curNode;

		fboOutput -> target -> set();

		for (auto &&poolPin : inputs){

			Node *curNodePool = pin -> node;

			if (curNodePool -> Type != NodeType::Buffer) continue;

			PoolNode *poolNode = reinterpret_cast<PoolNode *> curNodePool; //Slow but good for now
			Pool *curPool = poolNode -> pool;

			for (int i=0; i < curPool -> getSize(); i++){

				shaderObj -> setModel(value_ptr(curPool -> modelMats[i]));
				

			}

		}	

		
	}	




	visited = 1;
}





void bindAllNodes(){
	
	//find finalfbo

	//for all inputs of finalfbo
		//
}


void initialize(Node& newNode){

	if (newNode.type == NodeType::Shader){


	}

}






Pipeline::render_run(Node cur_node){

		
		//for all neighbors, add to priority queue with distance as priority

		//

		for (auto input in cur_node -> inputs){

			render_run(input -> node);
		}

		






		if (curr.type == pool){


		}	


	
	
}
