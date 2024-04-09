#include "Draw.h"

// ---------------------------FUNCTIONS---------------------------
// USED TO CLEAR THE SCREEN AND SET BACKGROUND COLOR IN UPDATE
void Clear ()
{
	// Background color
	glClearColor(0.f, 0.0f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// USED TO SET THE PROGRAM IN UPDATE
void UseProgram(GLuint core_program)
{
	glUseProgram(core_program);
	
}

// USED TO BIND VAO IN UPDATE
void DrawBindVAO(GLuint VAO)
{
	glBindVertexArray(VAO);
}

// USED TO END DRAWING IN UPDATE
void EndOfDrawing (GLFWwindow* window)
{
	glfwSwapBuffers(window);
	glFlush();

}

