#pragma once

#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <map>

namespace Blib {

GLuint loadTexture(const std::string &filename);
GLuint loadCubeMap(const std::vector<std::string> &faces);
GLuint loadCubeMap(const std::string &folder, const std::string& extention = ".png");

class Model {
public:
    void load(const char *path) { loadModel(path); }
    void draw(Blib::ShaderProgram &shader) const {
        for (const auto &mesh : meshes) {
            mesh.draw(shader);
        }
    }

private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::map<std::string, Texture> texturesLoaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

} // namespace Model
