#include <GLFW/glfw3.h>
extern GLFWwindow* window; 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/Controls.hpp"

extern glm::vec3 POSITION;


using namespace glm;


vec3 position = vec3(18.0, 18.0, 18.0); 
float horizontalAngle = 3.14f, verticalAngle = 0.0f;
float initialFoV = 45.0f;
float speed = 20.0f, mouseSpeed = 0.005f;

double xpos, ypos, xlast, ylast;

int init = 0;

int initControls(){
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetCursorPos(window, &xlast, &ylast);
	return 1;
}

mat4 computeMatricesFromInputs(){

	if (!init)
		init = initControls();

	static double lastTime = glfwGetTime(); //static local var = called once
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	glfwGetCursorPos(window, &xpos, &ypos);
	horizontalAngle += mouseSpeed * float(xlast - xpos);
	verticalAngle += mouseSpeed * float(ylast - ypos);
	xlast = xpos;
	ylast = ypos;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	vec3 right = vec3(sin(horizontalAngle - 3.14f/2.0f), 0, cos(horizontalAngle - 3.14f/2.0f));
	vec3 up = glm::cross(right, direction); //lol

	//Forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		position += direction * deltaTime * speed;
	//Backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		position -= direction * deltaTime * speed;
	//Right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		position += right * deltaTime * speed;
	//Left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		position -= right * deltaTime * speed;

	lastTime = currentTime;
	POSITION = position;

	return lookAt(position, position+direction, up);
}