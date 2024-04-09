#pragma once
#include "libs.h"

// --------------------------CAMERA GLOBAL VARIABLES--------------------------
// MODEL MATRIX VARIABLES
extern glm::vec3 position;
extern glm::vec3 rotation;
extern glm::vec3 scale;
extern glm::mat4 ModelMatrix;

// VIEW MATRIX VARIABLES
extern glm::vec3 worldUp;
extern glm::vec3 camFront;
extern glm::vec3 camPosition;
extern glm::mat4 ViewMatrix;

// PROJECTION MATRIX VARIABLES
extern float fov;
extern float nearPlane;
extern float farPlane;
extern glm::mat4 ProjectionMatrix;

// ------------------------------CAMERA FUNCTIONS------------------------------
void initCamera();
void uniformCameraMatrixes();
void initViewMatrix();
void initUniformsMatrixes(GLuint core_program);
void updateUniformsMatrixes(GLuint core_program);
void resetProjectionMatrix();
void getFrameBuffer(GLFWwindow* window, int& frameBufferWidth, int& frameBufferHeight);


