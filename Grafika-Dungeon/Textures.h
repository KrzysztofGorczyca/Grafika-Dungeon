#pragma once
#include "libs.h"

GLuint loadTexture(const char* path);
void configureTexture(GLuint texture);
void unbindTexture();
void updateUniforms(GLuint core_program, char const* uniformName);