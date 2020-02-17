#include <cstdlib> 
#include <cstdio>

#include <vector>

#include <regex> 
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>      // std::stringstream



using namespace std;


string read_shader(char* filePath){

    /*
    string code;
    ifstream shaderFile;

    shaderFile.open(shaderPath);

    stringstream shaderStream;

    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    return shaderStream.str(); 
    */ 
	/*
    std::ifstream shaderFile(shaderPath);
	
	if(!shaderFile){
	  //Error out here.
	}
	
	std::stringstream shaderData;
	
	shaderData << shaderFile.rdbuf();  //Loads the entire string into a string stream.
	shaderFile.close();
	
	return shaderData.str(); 
	*/
	std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + " ");
    }

    fileStream.close();
    return content; 
}
