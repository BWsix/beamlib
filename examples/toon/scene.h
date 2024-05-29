#pragma once

#include <beamlib.h>
#include "models.h"
#include "screen.h"

struct SceneToon : Blib::Scene {
    ToonBall lightball;
    Dragon dragon;
    ScreenToon screen;

    struct {
        glm::vec3 objectColor = glm::vec3(1.0);
        glm::vec3 lightColor = glm::vec3(1.0);

        float toonThreshold = 0.7;
    } config;

    struct {
        Blib::ShaderProgram phong;
        Blib::ShaderProgram toon;
        Blib::ShaderProgram light;
        Blib::ShaderProgram screen;
    } programs;

    void setup() override {
        glClearColor(0, 0, 0, 1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        Blib::camera.setTargetInstance(NULL);
        Blib::camera.setMovementSpeed(2);
        Blib::camera.transform.position = {0.0, 2.0, 5.0};
        Blib::camera.setPitch(-26.1);
        Blib::camera.resetYaw();
    }

    void init() override {
        lightball.transform.Scale({0.1, 0.1, 0.1});
        lightball.transform.Translate({1.0, 3.0, -3.0});

        dragon.transform.Scale({5, 5, 5});

        programs.phong = Blib::ResourceManager::LoadShader("toon-phong", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/phong.frag.glsl");
        programs.toon = Blib::ResourceManager::LoadShader("toon-toon", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/toon.frag.glsl");
        programs.light = Blib::ResourceManager::LoadShader("toon-light", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/light.frag.glsl");
        programs.screen = Blib::ResourceManager::GetShader("toon-screen");
    }

    static void LoadResources() {
        ToonBall::LoadResources();
        Dragon::LoadResources();
        ScreenToon::LoadResources();
    }

    void renderUI() override {
        ImGui::Begin("Config");

        ImGui::ColorEdit3("objectColor", &config.objectColor[0]);
        ImGui::ColorEdit3("lightColor", &config.lightColor[0]);
        ImGui::DragFloat("toonThreshold", &config.toonThreshold, 0.01, 0, 1);
        ImGui::Checkbox("Camera as Light Source", &camera_as_light_source);

        ImGui::SeparatorText("Objects");

        Blib::camera.RenderUI();
        if (ImGui::TreeNode("Light Source")) {
            lightball.transform.RenderUI("Light Source");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Object")) {
            dragon.transform.RenderUI("Object");
            ImGui::TreePop();
        }

        ImGui::End();
    }

    bool camera_as_light_source = true;
    void render() override {
        dragon.transform.RotateLocal({0.0, Blib::getDeltaTime(), 0.0});

        screen.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8, 0.4, 0.4, 1);

        auto& prog = programs.toon;
        prog.Use();
        prog.SetVec3("viewPos", Blib::camera.transform.getPosition());
        prog.SetVec3("objectColor", config.objectColor);
        prog.SetVec3("lightColor", config.lightColor);
        if (camera_as_light_source) {
            prog.SetVec3("lightPos", Blib::camera.transform.getPosition());
        } else {
            prog.SetVec3("lightPos", lightball.transform.getPosition());
        }

        dragon.render(prog);

        if (!camera_as_light_source) {
            programs.light.Use();
            programs.light.SetVec3("lightColor", config.lightColor);
            lightball.render(programs.light);
        }

        screen.unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        programs.screen.Use();
        programs.screen.SetFloat("threshold", config.toonThreshold);
        screen.render(programs.screen);
    }
};
