#pragma once

#include "shaderProgram.h"
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

struct Lighting {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Lighting lighting;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, Lighting lighting);
    void draw(Blib::ShaderProgram &shader) const;
    void drawInstanced(Blib::ShaderProgram &shader, int count) const;

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setupMesh();
};
