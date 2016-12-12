#ifndef Mesh_h
#define Mesh_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <Gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/types.h>

using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    GLuint id;
    string type;
    aiString path;
};

class Mesh
{
public:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) :
        vertices(vertices), indices(indices), textures(textures)
    {
        SetupMesh();
    }
    
    void Draw(Shader shader)
    {
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        
        for (GLuint i = 0; textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            stringstream ss;
            string number;
            string name = textures[i].type;
            
            if ("texture_diffuse" == name) {
                ss << diffuseNr++;
            } else if ("texture_specular" == name) {
                ss << specularNr++;
            }
            
            number = ss.str();
            
            glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        
        glUniform1f(glGetUniformLocation(shader.Program, "material.shinines"), 16.0f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        for (GLuint i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
private:
    GLuint VAO, VBO, EBO;

    void SetupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        
        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
        
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal) );
        
        // vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords) );
        
        glBindVertexArray(0);

    }
};

#endif /* Mesh_h */
