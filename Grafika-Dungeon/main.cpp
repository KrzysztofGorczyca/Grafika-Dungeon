// INCLUDES
#include "libs.h"
#include "Update.h"
#include "Draw.h"
#include "Inputs.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Options.h"
#include "Textures.h"
#include "Camera.h"

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

    // TEXTURES
    initializeTextures();

    initCamera();

    uniformCameraMatrixes();
    initViewMatrix();
    initUniformsMatrixes(core_program);


    // MAIN LOOP
    while(!glfwWindowShouldClose(window))
    {
        // UPDATE EVENT POLL
        glfwPollEvents();

        // UPDATE INPUT
        UpdateInput(window, position, rotation, scale);
        
        // DRAWING START
        Clear();
        glUseProgram(core_program);

        // UPDATE UNIFORMS
        updateUniformsOuter(core_program);


        // MATRIX UPDATE


        uniformCameraMatrixes();
        updateUniformsMatrixes(core_program);
        getFrameBuffer(window, frameBufferWidth, frameBufferHeight);
        resetProjectionMatrix();


        // ACTIVATE TEXTURES
        activateTextures();

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