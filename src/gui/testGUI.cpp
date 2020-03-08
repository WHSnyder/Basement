#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>



IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO &io = ImGui::GetIO();
// Setup Platform/Renderer bindings
ImGui_ImplGlfw_InitForOpenGL(window, true);
ImGui_ImplOpenGL3_Init(glsl_version);
// Setup Dear ImGui style
ImGui::StyleColorsDark();



ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
