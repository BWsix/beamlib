#pragma once

#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "beamlib.h"
#include "mesh.h"

namespace Model {

GLuint loadTexture(const std::string &filename);
GLuint loadCubeMap(const std::vector<std::string> &faces);

class Model {
public:
    void load(const char *path) { loadModel(path); }
    void Draw(beamlib::ShaderProgram &shader) const { for(const auto &mesh : meshes) mesh.Draw(shader); }

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
}
