#include "Inputs.h"


// ---------------------------FUNCTIONS---------------------------
void UpdateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	// EXIT BUTTON
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) && glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	// ---------MOVEMENT---------
	// FOWARD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z += 0.001f;
	}
	// BACKWARD
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z -= 0.001f;
	}
	// LEFT
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= 0.001f;
	}
	// RIGHT
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += 0.001f;
	}
	// UP
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position.y += 0.001f;
	}
	//DOWN
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		position.y -= 0.001f;
	}
	// ROTATE LEFT
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y += 0.01f;
	}
	// ROTATE RIGHT
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y -= 0.01f;
	}
}