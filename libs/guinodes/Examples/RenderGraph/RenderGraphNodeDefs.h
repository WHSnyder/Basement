#pragma once

#include <imgui_node_editor.h>
#include <ax/Math2D.h>
#include <ax/Builders.h>
#include <ax/Widgets.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>


enum class PinType{ Flow, Bool, Int, Float, Function, Delegate, Shader, Object, Buffer, String };
enum class PinKind{ Output, Input };
enum class NodeType{ Blueprint, Simple, Tree, Comment, Texture, Shader, Pool,  };
struct Node;
struct Link;
struct Pin{
	std::vector<Link *> links;
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
    int num, visited, static_flag;
    ImColor Color;
    NodeType Type;
    ImVec2 Size;
    std::string State, SavedState;
    virtual void process()=0;
    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255), NodeType type = NodeType::Blueprint):
        ID(id), Name(name), Color(color), Type(type), Size(0, 0), num(type == NodeType::Texture ? ++texture_cnt : 0)
    {}
};

struct Link{
    ed::LinkId ID;
    ed::PinId StartPinID, EndPinID;
    ImColor Color;
    Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId):
        ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
    {}
};