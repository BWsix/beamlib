#include "resourceManager.h"
#include "model.h"
#include "stb_image.h"

namespace Blib {

std::map<std::string, Texture> ResourceManager::Textures;
std::map<std::string, ShaderProgram> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;
std::map<std::string, json> ResourceManager::Animations;
std::map<std::string, GLuint> ResourceManager::GLuints;

const ShaderProgram& ResourceManager::LoadShader(std::string name, const char *vertexPath, const char *fragmentPath) {
    auto prog = ShaderProgram();
    prog.Compile(vertexPath, fragmentPath);
    return ResourceManager::Shaders[name] = prog;
}

const Model& ResourceManager::LoadModel(std::string name, const char *path) {
    auto m = Model();
    m.load(path);
    return ResourceManager::Models[name] = m;
}

void ResourceManager::Clear() {
    for (auto iter : Shaders) glDeleteProgram(iter.second.ID);
    for (auto iter : Textures) glDeleteTextures(1, &iter.second.ID);
}

Texture ResourceManager::loadTextureFromFile(const char *file, bool alpha) {
    Texture texture;

    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

} // namespace Blib
