#include <cstdlib> 
#include <cstdio>

#include <vector>

#include <regex> 
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;


string read_shader(string filepath){

	string content;
    ifstream fileStream(filepath.c_str());

    if(!fileStream.is_open()) {
        cerr << "Could not read file " << filepath << ". File does not exist." << endl;
        return "";
    }

    string line = "";
    stringstream shaderData;
    shaderData << fileStream.rdbuf();

    fileStream.close();
    
    content = shaderData.str();
    
    return content; 
}