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

    //TEXTURE
    int image_width = 0;
    int image_height = 0;
    unsigned char* image = SOIL_load_image("Textures/horse.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
		std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);


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

        //UPDATE UNIFORMS
        glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);

        //TEXTURE
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); 
        EndOfDrawing(window);	


        glBindVertexArray(0);
        //glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //EOP
    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteProgram(core_program);

    return 0;
}