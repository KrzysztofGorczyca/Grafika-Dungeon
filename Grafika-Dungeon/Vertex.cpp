#include "Vertex.h"

// --------------------------VERTEX GLOBAL VARIABLES--------------------------
Vertex vertices[] =
{
    //Position						    //Color				                //TexCoords                 //Normal
    glm::vec3(-0.5f,0.5f,0.f),		    glm::vec3(1.f,0.f,0.f),	    	    glm::vec2(0.f,1.f),         glm::vec3(0.f,0.f,1.f),
    glm::vec3(-0.5f,-0.5f,0.f),	        glm::vec3(0.f,1.f,0.f),	          	glm::vec2(0.f,0.f),         glm::vec3(0.f,1.f,1.f),
    glm::vec3(0.5f,-0.5f,0.f),		    glm::vec3(0.f,0.f,1.f),	        	glm::vec2(1.f,0.f),         glm::vec3(0.f,0.f,1.f),

    //Traingle 2 without duplicates
    glm::vec3(0.5f,0.5f,0.f),	            glm::vec3(1.f,1.f,0.f),	          	glm::vec2(1.f,1.f),         glm::vec3(0.f,0.f,1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
    0, 1, 2, // Triangle 1
    0, 2, 3 // Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

GLuint VAO; // Zmienna globalna

// ------------------------------VERTEX FUNCTIONS------------------------------
//FUNCTION FOR ENABLING BUFFERS IN INITIALIZATION
void enableBuffers()
{
    //Vertex Area Object
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

}	

//FUNCTION FOR ENABLING AND SETTING VERTEX ATRIBUTES POINTERS IN INITIALIZATION
void enableAndSetVertexAtributesPointers()
{
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);
    //Normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}
