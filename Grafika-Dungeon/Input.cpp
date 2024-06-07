#include <ctime>
#include <chrono>
#include "libs.h" // Dodaj nag³ówek dla bibliotek
#include "Player.h" // Dodaj nag³ówek dla klasy Player

bool isSoundPlaying = false;

void processInput(GLFWwindow* window, Camera& camera, float deltaTime, Player& player, bool menu, bool died)
{
    static float moveSoundTimer = 0.0f; // Zmienna do odmierzania czasu

    if (menu || died) return;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        moveSoundTimer += deltaTime; // Dodaj czas do odmierzacza
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        moveSoundTimer += deltaTime; // Dodaj czas do odmierzacza
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        moveSoundTimer += deltaTime; // Dodaj czas do odmierzacza
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
        moveSoundTimer += deltaTime; // Dodaj czas do odmierzacza
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        player.holdingE = true;
    }
    else {
        player.holdingE = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !player.isAnimating) {
        player.isAnimating = true;
        player.animationTime = 0.0f;
    }

    // Odtwarzaj dŸwiêk ruchu co 0.5 sekundy
    if (moveSoundTimer >= 0.5f && isSoundPlaying==false) {
        isSoundPlaying = true;
        playMoveSound(); // Funkcja odtwarzaj¹ca dŸwiêk ruchu
        moveSoundTimer = 0.0f; // Zresetuj odmierzacz czasu
        isSoundPlaying = false;
    }
}

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

void scroll_callback(GLFWwindow* window, Camera& camera, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}