#include "libs.h"

#include "Update.h"
#include "Draw.h"
#include "Inputs.h"
#include "Shaders.h"
#include "Vertex.h"


int main()
{
    //GLFW
    glfwInit();

    //SETTING OPENGL VERSION TO 4.4
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    //GLFWwindow
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 720;
    int frameBufferWidth = 0;
    int frameBufferHeight = 0;

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DUNGEON", NULL , NULL);

    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	glViewport(0,0,frameBufferWidth,frameBufferHeight);

    glfwMakeContextCurrent(window);


    //GLEW
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK)
    {
        std::cout << "ERROR::MAIN.CPP::GLEWINIT FAILED\n";
		glfwTerminate();
    }

    //OPENGL OPTIONS
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //SHADER_INIT
    GLuint core_program;
    if (!loadShaders(core_program))
        glfwTerminate();

 
    
    

    //Vertex Area Object
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    //Vertec Buffer Object

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, nrOfVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);


    //Element Buffer Object

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfIndices * sizeof(Vertex), indices, GL_STATIC_DRAW);


    //ENABLE AND SET VERTEX ATRIBUTES POINTERS (Input Assembly)

	//Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));
    glEnableVertexAttribArray(1);
    //Texcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    

    //MAIN LOOP
    while(!glfwWindowShouldClose(window))
    {
        //UPDATE INPUTS
        glfwPollEvents();

        //UPDATE
        UpdateInput(window);
        

        //DRAWING
        Clear();
        glUseProgram(core_program);
        glBindVertexArray(VAO);
        //std::cout << nrOfVertices << "\n";
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