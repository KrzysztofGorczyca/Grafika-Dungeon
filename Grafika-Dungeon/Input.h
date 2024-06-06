#pragma once
#include "libs.h"
#include "Camera.h"
#include "Player.h"

void processInput(GLFWwindow* window, Camera& camera, float deltaTime, Player& player);
void mouse_callback(GLFWwindow* window, Camera& camera, double xposIn, double yposIn, bool firstMouse, float lastX, float lastY);
void scroll_callback(GLFWwindow* window, Camera& camera, double xoffset, double yoffset);