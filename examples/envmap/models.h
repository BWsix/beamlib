#pragma once

#include <beamlib.h>

class Cybertruck : public Blib::Instance {
public:
    using Blib::Instance::Instance;
    // void render(Blib::ShaderProgram& prog) {
    //     prog.SetMat4("model", transform.getModelMatrix());
    //     prog.SetMat4("view", Blib::camera.getViewMatrix());
    //     prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
    //
    //     auto obj = Blib::ResourceManager::GetModel("envmapdemo-cybertruck");
    //     obj.draw(prog);
    // }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("envmapdemo-cybertruck", "models/cybertruck/cybertruck.obj");
    }
};
