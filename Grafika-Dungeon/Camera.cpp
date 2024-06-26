#include "Camera.h"
#include "Shaders.h"

// MODEL MATRIX VARIABLES
glm::vec3 position;
glm::vec3 rotation;
glm::vec3 scale;
glm::mat4 ModelMatrix;

// VIEW MATRIX VARIABLES
glm::vec3 worldUp;
glm::vec3 camFront;
glm::vec3 camPosition;
glm::mat4 ViewMatrix;

// PROJECTION MATRIX VARIABLES
float fov;
float nearPlane;
float farPlane;
glm::mat4 ProjectionMatrix;

// FUNCTIONS

void initCamera() {
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    worldUp = glm::vec3(0.f, 1.f, 0.f);
    camFront = glm::vec3(0.f, 0.f, -1.f);
    camPosition = glm::vec3(0.f, 0.f, 1.f);
    fov = 90.0f;
    nearPlane = 0.1f;
    farPlane = 100.0f;
    ViewMatrix = glm::mat4(1.f);
}

void initViewMatrix() {
    ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
}

void uniformCameraMatrixes() {
    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(position));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, scale);

    ProjectionMatrix = glm::mat4(1.f);
    ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(1024) / 720, nearPlane, farPlane);
}

glm::vec3 lightPos0(0.f, 0.f, 2.f);

void initUniformsMatrixes(GLuint core_program) {
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    glUniform3fv(glGetUniformLocation(core_program, "lightPos0"), 1, glm::value_ptr(lightPos0));
    glUniform3fv(glGetUniformLocation(core_program, "cameraPos"), 1, glm::value_ptr(camPosition));
}

void updateUniformsMatrixes(GLuint core_program) {
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
}

void resetProjectionMatrix() {
    ProjectionMatrix = glm::mat4(1.f);
    ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(1024) / 720, nearPlane, farPlane);
}

void getFrameBuffer(GLFWwindow* window, int& frameBufferWidth, int& frameBufferHeight) {
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
}
