#include <cstdlib> 
#include <cstdio>

#include <vector>

#include <regex> 
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>      // std::stringstream

using namespace std;

extern string basepath;



string read_shader(string filepath){

    filepath.insert(0, basepath);

	std::string content;
    std::ifstream fileStream(filepath.c_str());

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filepath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    std::stringstream shaderData;

    #ifdef MACOS
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + " ");
    }
    #else
    shaderData << fileStream.rdbuf();
    #endif

    fileStream.close();
    content = shaderData.str();
    return content; 
}
