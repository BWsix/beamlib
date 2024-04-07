#pragma once

#include "model.h"
#include "shaderProgram.h"
#include "texture.h"
#include <map>
#include <string>

namespace Blib {

class ResourceManager {
public:
    // resource storage
    static std::map<std::string, ShaderProgram> Shaders;
    static std::map<std::string, Texture> Textures;
    static std::map<std::string, Model> Models;

    static ShaderProgram LoadShader(std::string name, const char *vertexPath, const char *fragmentPath);
    static ShaderProgram GetShader(std::string name) { return Shaders[name]; }
    static Texture LoadTexture(std::string name, const char *file, bool alpha) { return Textures[name] = loadTextureFromFile(file, alpha); }
    static Texture GetTexture(std::string name) { return Textures[name]; }
    static Model LoadModel(std::string name, const char *path);
    static Model GetModel(std::string name) { return Models[name]; }
    static void Clear();

private:
    // private constructor, that is we do not want any actual resource manager
    // objects. Its members and functions should be publicly available (static).
    ResourceManager() {}
    // loads and generates a shader from file
    static ShaderProgram loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // loads a single texture from file
    static Texture loadTextureFromFile(const char *file, bool alpha);
};

} // namespace Blib
