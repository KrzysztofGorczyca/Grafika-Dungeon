#include "Draw.h"


void Clear ()
{
	// Background color
	glClearColor(0.f, 0.0f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void UseProgram(GLuint core_program)
{
	glUseProgram(core_program);
	
}

void Draw_BindVAO(GLuint VAO)
{
	glBindVertexArray(VAO);
}

void EndOfDrawing (GLFWwindow* window)
{
	glfwSwapBuffers(window);
	glFlush();

}

