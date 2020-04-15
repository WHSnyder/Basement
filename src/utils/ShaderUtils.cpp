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
    std::ifstream fileStream(filepath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filepath << ". File does not exist." << std::endl;
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
