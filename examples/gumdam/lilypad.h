#pragma once

#include <beamlib.h>

class Lilypad {

public:
    Blib::Instance root{"lilypad-root", Blib::ResourceManager::GetModel("lilypad")};

    void render() {
        auto prog = Blib::ResourceManager::GetShader("gumdam-lazer");
        prog.Use();
        prog.SetMat4("model", glm::mat4(1));
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        root.Render(prog);
    }

    void renderUI() {
        root.RenderUI();
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("lilypad", "models/lilypad/lilypad.obj");
    }
};
