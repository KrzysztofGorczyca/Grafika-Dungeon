#pragma once
#include "libs.h"
#include "Camera.h"
#include "Player.h"

/**
 * @brief Processes input from the keyboard and mouse for controlling the camera and player.
 *
 * @param window Pointer to the GLFW window.
 * @param camera Reference to the Camera object.
 * @param deltaTime Time difference between the current frame and the previous frame.
 * @param player Reference to the Player object.
 * @param menu Indicates if the menu is currently active.
 * @param died Indicates if the player has died.
 */
void processInput(GLFWwindow* window, Camera& camera, float deltaTime, Player& player, bool menu, bool died);

/**
 * @brief Callback function to handle mouse movement.
 *
 * @param window Pointer to the GLFW window.
 * @param camera Reference to the Camera object.
 * @param xposIn Current X position of the mouse.
 * @param yposIn Current Y position of the mouse.
 * @param firstMouse Indicates if it's the first time the mouse is being moved.
 * @param lastX Last X position of the mouse.
 * @param lastY Last Y position of the mouse.
 */
void mouse_callback(GLFWwindow* window, Camera& camera, double xposIn, double yposIn, bool firstMouse, float lastX, float lastY);

/**
 * @brief Callback function to handle mouse scroll.
 *
 * @param window Pointer to the GLFW window.
 * @param camera Reference to the Camera object.
 * @param xoffset Scroll offset along the X axis.
 * @param yoffset Scroll offset along the Y axis.
 */
void scroll_callback(GLFWwindow* window, Camera& camera, double xoffset, double yoffset);