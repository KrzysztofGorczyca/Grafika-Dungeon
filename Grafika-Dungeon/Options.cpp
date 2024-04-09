#include "Options.h"

void setupOpenGL(GLFWwindow*& window, int width, int height, const char* title) {
    // GLFW
    glfwInit();

    // Setting OpenGL version to 4.4
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    int frameBufferWidth = 0;
    int frameBufferHeight = 0;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    glfwMakeContextCurrent(window);
}

void setupGLEW() {
    // GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR::OPTIONS.CPP::GLEWINIT FAILED\n";
        glfwTerminate();
    }
}

void setupOpenGLOptions() {
    // OpenGL Options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

