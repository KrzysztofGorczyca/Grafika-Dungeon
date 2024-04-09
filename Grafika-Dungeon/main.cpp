#include "libs.h"

#include "Update.h"
#include "Draw.h"
#include "Inputs.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Options.h"


int main()
{
    //WINDOW INITIALIZATION 
    GLFWwindow* window;

    //SETUP OPTIONS
    setupOpenGL(window, 1024, 720, "Dungeon");
    setupGLEW();
    setupOpenGLOptions();

    //SHADER_INIT
    GLuint core_program;
    if (!loadShaders(core_program))
        glfwTerminate();

    //VERTEX DATA
    enableBuffers();
    enableAndSetVertexAtributesPointers();

    //MAIN LOOP
    while(!glfwWindowShouldClose(window))
    {
        //UPDATE EVENT POLL
        glfwPollEvents();

        //UPDATE INPUT
        UpdateInput(window);
        

        //DRAWING
        Clear();
        glUseProgram(core_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); 
        EndOfDrawing(window);	
    }

    //EOP
    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteProgram(core_program);

    return 0;
}