#pragma once
#include <fstream>

#include "libs.h"

class Shader
{
    GLuint id;

private:
    std::string loadShaderSource(const char* filename)
    {
        std::string temp = "";
        std::string src = "";

        std::ifstream in_file;

        // Open file
        in_file.open(filename);

        if (in_file.is_open())
        {
            while (std::getline(in_file, temp))
                src += temp + "\n";
        }
        else
        {
            std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE " << filename << std::endl;
        }
        in_file.close();

        return src;
    }

    GLuint loadShader(GLenum type, const char* filename)
    {
        char infoLog[512];
        GLint success;

        GLuint shader = glCreateShader(type);
        std::string str_src = this->loadShaderSource(filename);
        const GLchar* src = str_src.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        // Check for errors
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER " << filename << std::endl;
            std::cout << infoLog << std::endl;
        }

        return shader;
    }

    void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
    {
        char infoLog[512];
        GLint success;

        this->id = glCreateProgram();

        glAttachShader(this->id, vertexShader);

        if (geometryShader)
            glAttachShader(this->id, geometryShader);

        glAttachShader(this->id, fragmentShader);

        glLinkProgram(this->id);

        // Check for errors
        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << std::endl;
            std::cout << infoLog << std::endl;
        }
        glUseProgram(0);
    }

public:
    Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr)
    {
        GLuint vertexShader = 0;
        GLuint geometryShader = 0;
        GLuint fragmentShader = 0;

        vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
        fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

        if (geometryFile != nullptr)
            geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);

        linkProgram(vertexShader, geometryShader, fragmentShader);

        // Delete shaders after linking
        glDeleteShader(vertexShader);
        if (geometryFile != nullptr)
            glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader()
    {
        glDeleteProgram(this->id);
    }

    void use()
    {
        glUseProgram(this->id);
    }

    GLuint getID() const
    {
        return this->id;
    }
};
