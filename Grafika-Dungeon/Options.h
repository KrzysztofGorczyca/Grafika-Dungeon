#pragma once
#include "libs.h"

extern int frameBufferWidth;
extern int frameBufferHeight;

void setupOpenGL(GLFWwindow*& window, int width, int height, const char* title);
void setupGLEW();
void setupOpenGLOptions();