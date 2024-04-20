#pragma once

#include <beamlib.h>

class Gymbar {

public:
    Blib::Instance root{"gymbar-root", Blib::ResourceManager::GetModel("gymbar-bar")};

    Gymbar() {}

    void render() {
        auto prog = Blib::ResourceManager::GetShader("gymbar");
        prog.Use();
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        root.Render(prog);
    }

    void renderUI() {
        root.RenderUI();
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("gymbar", "shaders/default.vert.glsl", "shaders/default.frag.glsl");
        Blib::ResourceManager::LoadModel("gymbar-bar", "models/gymbar/gymbar.obj");
    }
};
