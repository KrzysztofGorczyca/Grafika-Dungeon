#pragma once
#include "libs.h"

void Clear();
void UseProgram(GLuint core_program);
void Draw_BindVAO(GLuint VAO);
void Draw();
void DrawTriangleFromIndices(unsigned nrOfIndicies);
void EndOfDrawing(GLFWwindow* window);