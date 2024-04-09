#include "Textures.h"
#include "libs.h"

GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

    if (image) {
        configureTexture(textureID);
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(image);
    }
    else {
        std::cerr << "ERROR::TEXTURES.CPP::TEXTURE_LOADING_FAILED" << std::endl;
    }

    return textureID;
}

void configureTexture(GLuint texture) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void unbindTexture() {
    glBindVertexArray(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void updateUniforms(GLuint core_program, char const* uniformName) {
    glUniform1i(glGetUniformLocation(core_program, uniformName), 0);
}
