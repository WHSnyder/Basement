#include <cstdlib> 
#include <cstdio>

#include <vector>

#include <regex> 
#include <string>

#include <fstream>
#include <iostream>


using namespace std;


string read_shader(const char* shaderPath){

    string code;
    ifstream shaderFile;

    shaderFile.open(shaderPath);

    stringstream shaderStream;

    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    return shaderStream.str();    
}
