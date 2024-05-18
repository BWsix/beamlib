#pragma once

#include "model.h"
#include "object.h"
#include "shaderProgram.h"
#include "texture.h"
#include "utils.h"
#include <map>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <iostream>

namespace Blib {

class ResourceManager {
public:
    // resource storage
    static std::map<std::string, ShaderProgram> Shaders;
    static std::map<std::string, Texture> Textures;
    static std::map<std::string, Model> Models;
    static std::map<std::string, json> Animations;
    static std::map<std::string, GLuint> GLuints;

    static const ShaderProgram& LoadShader(std::string name, const char *vertexPath, const char *fragmentPath);
    static ShaderProgram& GetShader(std::string name) {
        auto& prog = Shaders.at(name); 
        prog.Use();
        return prog;
    }
    static const Texture& LoadTexture(std::string name, const char *file, bool alpha) { return Textures[name] = loadTextureFromFile(file, alpha); }
    static const Texture& GetTexture(std::string name) { return Textures.at(name); }
    static const Model& LoadModel(std::string name, const char *path);
    static const Model& GetModel(std::string name) { return Models.at(name); }
    static const json& LoadAnimation(std::string name, const char *path) {
        std::cout << "Loading Animation: " + name + "\n";
        return Animations[name] = json::parse(SlurpFile(path)); 
    }
    static const json& GetAnimation(std::string name) { return Animations[name]; }
    static GLuint& GetGLuint(std::string name) { return GLuints[name]; }
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
