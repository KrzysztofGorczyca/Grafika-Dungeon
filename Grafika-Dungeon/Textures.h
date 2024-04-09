#pragma once
#include "libs.h"

// TEXTURES
extern GLuint texture0;
extern GLuint texture1;

// FUNCTIONS
GLuint loadTexture(const char* path);
void configureTexture();
void unbindTexture();
void updateUniformsOuter(GLuint core_program);
void updateUniformsInner(GLuint core_program, char const* uniformName, int index);
void activateTexture(GLuint texture, int unitIndex);
void activateTextures();
void initializeTextures();

