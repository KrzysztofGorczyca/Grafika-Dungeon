#include "Shaders.h"
#include <fstream>

bool loadShaders(GLuint &program)
{
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//vertex
	in_file.open("vertex_core.glsl");

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
		{
			src += temp + "\n";
		}
	}
	else
		std::cout << "ERROR::SHADERS.CPP::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE\n";

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERS.CPP::LOADSHADERS::COULD_NOT_LOAD_VERTEX_SHADER\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	temp = "";
	src = "";

	

	//Fragment

	in_file.open("fragment_core.glsl");

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "ERROR::SHADERS.CPP::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE\n";
		loadSuccess = false;
	}
	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERS.CPP::LOADSHADERS::COULD_LOAD_NOT_FRAGMENT_SHADER\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	//Program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program,GL_LINK_STATUS,&success);

	if(!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERS.CPP::LOADSHADERS::COULD_NOT_LINK_PROGRAM\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	//End
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);

	return loadSuccess;
}

