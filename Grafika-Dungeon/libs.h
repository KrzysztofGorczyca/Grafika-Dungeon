#pragma once
#include <iostream>
#include <string>

//GLEW
#include <glew.h>

//GLFW
#include <glfw3.h>

//GLM
#include <glm.hpp>
#include <vec3.hpp>
#include <vec2.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//SOIL
#include <SOIL2.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoords;
};