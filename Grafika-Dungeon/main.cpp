// INCLUDES
#include "libs.h"
#include "Update.h"
#include "Draw.h"
#include "Inputs.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Options.h"
#include "Textures.h"

// MAIN FUNCTION
int main()
{
    // WINDOW INITIALIZATION 
    GLFWwindow* window;

    // SETUP OPTIONS
    setupOpenGL(window, 1024, 720, "Dungeon");
    setupGLEW();
    setupOpenGLOptions();

    // SHADER_INIT
    GLuint core_program;
    if (!loadShaders(core_program))
        glfwTerminate();

    // VERTEX DATA
    enableBuffers();
    enableAndSetVertexAtributesPointers();

    // TEXTURE
    GLuint texture0 = loadTexture("Textures/horse.png");


    // MAIN LOOP
    while(!glfwWindowShouldClose(window))
    {
        // UPDATE EVENT POLL
        glfwPollEvents();

        // UPDATE INPUT
        UpdateInput(window);
        
        // DRAWING START
        Clear();
        glUseProgram(core_program);

        // UPDATE UNIFORMS
        updateUniforms(core_program, "texture0");

        // TEXTURE
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        // DRAW OBJECT
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
        EndOfDrawing(window);	

        // UNBIND TEXTURE
        unbindTexture();
    }

    // EOP
    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteProgram(core_program);

    return 0;
}