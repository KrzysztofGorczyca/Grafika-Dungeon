#pragma once
#include "libs.h"

// TEXTURES


// FUNCTIONS
GLuint loadTexture(const char* path);
void configureTexture(GLuint texture);
void unbindTexture();
void updateUniforms(GLuint core_program, char const* uniformName, int index);
void activateTexture(GLuint texture, int unitIndex);
void initializeTextures();

