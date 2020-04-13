#include <GLFW/glfw3.h>
extern GLFWwindow* window; 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "utils/Controls.hpp"

mat4 ViewMatrix;
mat4 ProjectionMatrix;

mat4 getViewMatrix(){
	return ViewMatrix;
}
mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
vec3 position = vec3(18.0, 18.0, 18.0); 

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;

// Initial vertical angle : none
float verticalAngle = 0.0f;

// Initial Field of View
float initialFoV = 45.0f;

float speed = 20.0f; 
float mouseSpeed = 0.005f;



mat4 computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	vec3 right = vec3(sin(horizontalAngle - 3.14f/2.0f), 0, cos(horizontalAngle - 3.14f/2.0f));
	vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;

	return lookAt(position,           // Camera is here
					   position+direction, // and looks here : at the same position, plus "direction"
					   up                  // Head is up (set to 0,-1,0 to look upside-down)
					   );

}