// -------------------------------INCLUDES-------------------------------------
#include "libs.h"
#include "Update.h"
#include "Draw.h"
#include "Inputs.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Options.h"
#include "Textures.h"
#include "Camera.h"
#include "Material.h"
// ----------------------------MAIN FUNCTION-----------------------------------
int main()
{
    // WINDOW INITIALIZATION 
    GLFWwindow* window;

    // SETUP OPTIONS
    setupOpenGL(window, 1024, 720, "Dungeon");
    setupGLEW();
    setupOpenGLOptions();

    // SHADER_INIT

    Shader core_program("vertex_core.glsl", "fragment_core.glsl");
    GLuint core_program;

    // VERTEX DATA
    enableBuffers();
    enableAndSetVertexAtributesPointers();

    // TEXTURES
    Texture texture0("Textures/horse.png", GL_TEXTURE_2D, 0);
    texture0.bind();
    Texture texture1("Textures/texture_example.png", GL_TEXTURE_2D, 1);
    texture0.bind();

    // MATERIALS
    Material material0(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), texture0.getTextureUnit(), texture1.getTextureUnit());

    // CAMERA AND MATRIX INITIALIZATION
    initCamera();
    uniformCameraMatrixes();
    initViewMatrix();
    initUniformsMatrixes(core_program);

    //INIT UNIFORMS
    core_program.use();

    // ----------------------------MAIN LOOP-----------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // UPDATE EVENT POLL
        glfwPollEvents();

        // UPDATE INPUT
        UpdateInput(window, position, rotation, scale);
        
        // DRAWING START
        Clear();
        UseProgram(core_program);

        // UPDATE UNIFORMS
        updateUniformsOuter(core_program);
        material0.sendToShader(core_program);

        // CAMERA AND MATRIX UPDATE
        uniformCameraMatrixes();
        updateUniformsMatrixes(core_program);
        getFrameBuffer(window, frameBufferWidth, frameBufferHeight);
        resetProjectionMatrix();

        // ACTIVATE TEXTURES

        // DRAW OBJECT
        DrawBindVAO(VAO);
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
        EndOfDrawing(window);	

        // UNBIND TEXTURE
        //texture0.unbind(GL_TEXTURE_2D);
        //unbindTexture();
    }

    // EOP
    glfwDestroyWindow(window);
    glfwTerminate();
    glUseProgram(0);
    glDeleteProgram(core_program);

    return 0;
}