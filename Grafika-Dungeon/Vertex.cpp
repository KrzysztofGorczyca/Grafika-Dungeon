#include "Vertex.h"

Vertex vertices[] =
{
    //Position								//Color								//TexCoords
    glm::vec3(-0.5f,0.5f,0.f),		        glm::vec3(1.f,0.f,0.f),	    	    glm::vec2(0.f,1.f),
    glm::vec3(-0.5f,-0.5f,0.f),	            glm::vec3(0.f,1.f,0.f),	          	glm::vec2(0.f,0.f),
    glm::vec3(0.5f,-0.5f,0.f),		        glm::vec3(0.f,0.f,1.f),	        	glm::vec2(1.f,0.f),

    //Traingle 2 bez duplikatow
    glm::vec3(0.5f,0.5f,0.f),	            glm::vec3(1.f,1.f,0.f),	          	glm::vec2(0.f,0.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
    0, 1, 2, // Triangle 1
    0, 2, 3 // Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);
