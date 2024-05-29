#pragma once

#include <beamlib.h>
#include "models.h"
#include "skybox.h"

struct EnvMapScene : Blib::Scene {
    Cybertruck cybertruck = Cybertruck("CyberTruck", Blib::ResourceManager::GetModel("envmapdemo-cybertruck"));
    EnvMapDemo::Skybox skybox;
    Blib::ShaderProgram phong;

    void init() override {
        phong = Blib::ResourceManager::LoadShader("envmapdemo-phong", "examples/envmap/shaders/default.vert.glsl", "examples/envmap/shaders/phong.frag.glsl");

        cybertruck.transform.Scale({10, 10, 10});
    }

    void setup() override {
        Blib::camera.setTargetInstance(NULL);
        Blib::camera.setMovementSpeed(20);
        Blib::camera.transform.position = {-38.276, 5.635, -13.720};
        Blib::camera.setPitch(-3.150);
        Blib::camera.setYaw(-338.049);

        glClearColor(0, 0, 0, 1);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    static void LoadResources() {
        Cybertruck::LoadResources();
        EnvMapDemo::Skybox::LoadResources();
    }

    void render() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, Blib::WIDTH, Blib::HEIGHT);

        glDepthFunc(GL_LEQUAL);
        auto& prog = phong;
        prog.Use();
        prog.SetVec3("viewPos", Blib::camera.transform.getPosition());
        prog.SetVec3("lightPos", Blib::camera.transform.getPosition());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        cybertruck.transform.RotateLocal({0, Blib::getDeltaTime(), 0});
        cybertruck.Render(prog);

        skybox.render();
    }

    void renderUI() override {
        ImGui::Begin("Config");
        cybertruck.RenderUI();
        Blib::camera.RenderUI();
        ImGui::End();
    }
};

