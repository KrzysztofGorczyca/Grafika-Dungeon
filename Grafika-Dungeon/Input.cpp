#include <ctime>
#include <chrono>
#include "libs.h"      ///< Header for external libraries
#include "Player.h"    ///< Header for the Player class

bool isSoundPlaying = false; ///< Indicates if a sound is currently playing

/**
 * @brief Processes input from the keyboard and mouse to control the camera and player actions.
 *
 * @param window The GLFW window context.
 * @param camera The camera object to control.
 * @param deltaTime The time elapsed between the current and previous frame.
 * @param player The player object to control.
 * @param menu Indicates if the menu is currently active.
 * @param died Indicates if the player has died.
 */
void processInput(GLFWwindow* window, Camera& camera, float deltaTime, Player& player, bool menu, bool died)
{
    static float moveSoundTimer = 0.0f; ///< Timer for managing move sound intervals

    if (menu || died) return;

    // If the player presses the ESC key, close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera if the player presses W, S, A, or D keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        moveSoundTimer += deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        moveSoundTimer += deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        moveSoundTimer += deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
        moveSoundTimer += deltaTime;
    }

    // Set holdingE to true if the player presses the E key
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        player.holdingE = true;
    }
    else {
        player.holdingE = false;
    }

    // Trigger player animation if the left mouse button is pressed and the player is not already animating
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !player.isAnimating) {
        player.isAnimating = true;
        player.animationTime = 0.0f;
    }

    // Play move sound every 0.5 seconds
    if (moveSoundTimer >= 0.5f && isSoundPlaying == false) {
        isSoundPlaying = true;
        playMoveSound(); ///< Function to play move sound
        moveSoundTimer = 0.0f; ///< Reset the move sound timer
        isSoundPlaying = false;
    }
}

/**
 * @brief Handles mouse movement to control the camera.
 *
 * @param window The GLFW window context.
 * @param camera The camera object to control.
 * @param xposIn The current x position of the mouse.
 * @param yposIn The current y position of the mouse.
 * @param firstMouse Indicates if this is the first mouse movement.
 * @param lastX The last x position of the mouse.
 * @param lastY The last y position of the mouse.
 */
void mouse_callback(GLFWwindow* window, Camera& camera, double xposIn, double yposIn, bool firstMouse, float lastX, float lastY)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

/**
 * @brief Handles mouse scroll to control the camera zoom.
 *
 * @param window The GLFW window context.
 * @param camera The camera object to control.
 * @param xoffset The scroll offset in the x direction.
 * @param yoffset The scroll offset in the y direction.
 */
void scroll_callback(GLFWwindow* window, Camera& camera, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
