#pragma once

#include "mesh.h"
#include <vector>
#include <GL/gl.h>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Loader {
public:
    static void LoadResources() {
        Blib::ResourceManager::LoadTexture("gold", "textures/gold.jpg", false);
        Blib::ResourceManager::LoadShader("dragon", "examples/water/shaders/gragon.vert.glsl", "examples/water/shaders/gragon.frag.glsl");
    }

    void init() {
        auto j = json::parse(Blib::SlurpFile("examples/water/dragon.json"));
        saved_meshes.clear();
        for (const auto& m : j["meshes"]) {
            saved_meshes.push_back(MyMesh::Load(m));
        }
    }

    std::vector<MyMesh> saved_meshes;
    void renderSavedMeshes(glm::mat4 m, bool reflection = false) {
        auto prog = Blib::ResourceManager::GetShader("dragon");
        prog.SetBool("reflectionMode", reflection);

        prog.SetMat4("model", m);
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        prog.SetVec3("viewPos", Blib::camera.getPosition());
        prog.SetVec3("lightPos", Blib::camera.getPosition());

        prog.SetVec3("lightColor", {1, 1, 1});
        prog.SetVec3("objectColor", {1, 1, 1});

        prog.SetInt("diffuse", 19);
        for (const auto& mesh : saved_meshes) {
            glActiveTexture(GL_TEXTURE19);
            Blib::ResourceManager::GetTexture("gold").Bind();
            mesh.render();
        }
    }
};
