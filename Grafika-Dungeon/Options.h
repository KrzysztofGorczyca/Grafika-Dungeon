#pragma once
#include "libs.h"
// -------------------------- GLOBAL VARIABLES--------------------------
extern int frameBufferWidth;
extern int frameBufferHeight;

// ------------------------------ FUNCTIONS------------------------------
void setupOpenGL(GLFWwindow*& window, int width, int height, const char* title);
void setupGLEW();
void setupOpenGLOptions();