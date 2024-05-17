#pragma once

#include <beamlib.h>

struct Ball {
    Blib::Transform transform;

    void render(glm::vec3 lightColor) {
        auto prog = Blib::ResourceManager::GetShader("lightball");
        prog.Use();
        prog.SetVec3("lightColor", lightColor);
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto ball = Blib::ResourceManager::GetModel("ball");
        ball.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("ball", "models/misc/ball.obj");
        Blib::ResourceManager::LoadShader("lightball", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/light.frag.glsl");
    }
};
