#include "Textures.h"

// REPETITIVE FUNCTIONS
GLuint texture0;
GLuint texture1;

// ADD NEW TEXTURES HERE
void initializeTextures() {
    texture0 = loadTexture("Textures/horse.png");
    texture1 = loadTexture("Textures/texture_example.png");
}

void updateUniformsOuter(GLuint core_program) {
    updateUniformsInner(core_program, "texture0", 0);
    updateUniformsInner(core_program, "texture1", 1);
}

void activateTextures() {
	activateTexture(texture0, 0);
	activateTexture(texture1, 1);
}

// STATIC FUNCTIONS

GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

    if (image) {
        configureTexture();
        
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

void configureTexture() {
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

void updateUniformsInner(GLuint core_program, char const* uniformName, int index) {
    glUniform1i(glGetUniformLocation(core_program, uniformName), index);
}

void activateTexture(GLuint texture, int unitIndex) {
	glActiveTexture(GL_TEXTURE0 + unitIndex);
	glBindTexture(GL_TEXTURE_2D, texture);
}
