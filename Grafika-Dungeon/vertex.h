#pragma once
#include "libs.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoords;
};

extern Vertex vertices[];

extern unsigned nrOfVertices;

GLuint indices[];
extern unsigned nrOfIndices;

void enableBuffers();
