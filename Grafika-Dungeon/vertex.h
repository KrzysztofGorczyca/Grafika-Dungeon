#pragma once
#include "libs.h"
// --------------------------VERTEX GLOBAL VARIABLES--------------------------
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
};

extern Vertex vertices[];

extern unsigned nrOfVertices;

GLuint indices[];
extern unsigned nrOfIndices;
extern GLuint VAO;

// ------------------------------VERTEX FUNCTIONS------------------------------
void enableBuffers();
void enableAndSetVertexAtributesPointers();

