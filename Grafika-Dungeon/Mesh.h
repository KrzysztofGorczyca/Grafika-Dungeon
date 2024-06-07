#pragma once
#include "libs.h"
#include "Vertex.h"
using namespace std;

/**
 * @brief Structure to represent a texture.
 */
struct Texture {
    unsigned int id; ///< The ID of the texture.
    string type; ///< The type of the texture (e.g., diffuse, specular).
    string path; ///< The path to the texture file.
};

/**
 * @brief Class to represent a 3D mesh.
 */
class Mesh {
public:
    // mesh data
    vector<Vertex> vertices; ///< Vector of vertices in the mesh.
    vector<unsigned int> indices; ///< Vector of indices for the mesh.
    vector<Texture> textures; ///< Vector of textures used by the mesh.

    /**
     * @brief Constructor to initialize the mesh with vertices, indices, and textures.
     *
     * @param vertices Vector of vertices.
     * @param indices Vector of indices.
     * @param textures Vector of textures.
     */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    /**
     * @brief Draws the mesh using the given shader.
     *
     * @param shader Reference to the shader program.
     */
    void Draw(Shader& shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO; ///< Vertex Array Object, Vertex Buffer Object, and Element Buffer Object.

    /**
     * @brief Sets up the mesh by initializing the buffers and vertex attributes.
     */
    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
            &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};