#pragma once

#include <GL/glew.h>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
#include <time.h>

#include <sys/types.h>
#include <unistd.h>


#include <GLFW/glfw3.h>

#include "rendering/Shader.h"



GLuint64 getTimer(int block);
void setTimer();
int step_game(float timestep);
int initialize_window();
void destroy_context();
void SSBOToTexture(Shader *ssbo2tex, GLuint ssbo, GLuint texID);
void textureToSSBO(Shader *tex2ssbo, GLuint texID, GLuint ssbo);