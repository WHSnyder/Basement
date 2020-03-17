#include <stack>

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




}





void ShaderNode::process(){
	
	//set current program
		
	//for each output fbo
		//for each pool
			//bind vaos, uniforms
			//render

	glUseProgram(shaderObj -> progID);


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
