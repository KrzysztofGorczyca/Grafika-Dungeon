#include "Inputs.h"

void UpdateInput(GLFWwindow* window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) && glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}