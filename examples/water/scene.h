#pragma once

#include "loader.h"
#include "models.h"
#include "screen.h"
#include "skybox.h"
#include <beamlib.h>

struct WaterScene : Blib::Scene {
    WaterDemo::Skybox skybox;
    WaterDemo::Grid water;
    WaterDemo::Ball lightball;
    WaterDemo::Dragon dragon;
    WaterDemo::Torus torus;
    WaterDemo::Screen screen = WaterDemo::Screen(Blib::WIDTH, Blib::HEIGHT);

    Blib::ShaderProgram phong;
    Blib::ShaderProgram screenProgram;

    Loader l;

    bool fresnel_test = false;

    void setup() override {
        glEnable(GL_DEPTH_TEST);

        Blib::camera.setTargetInstance(NULL);
        Blib::camera.transform.position = {0.0, 1.0, 20.0};
        Blib::camera.resetPitch();
        Blib::camera.resetYaw();
    }

    void init() override {
        l.init();

        water.transform.Scale({5, 1, 5});
        water.transform.Translate({0, -1, 0});

        lightball.transform.Translate({5, 5, -5});

        dragon.transform.Scale({5, 5, 5});
        dragon.transform.Translate({0, 1, 0});

        torus.transform.Translate({0, -10, 30});
        torus.transform.RotateLocal(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)));

        screen.loadResources();

        screenProgram.Compile("shaders/screen.vert.glsl", "examples/water/shaders/screen.frag.glsl");

        phong = Blib::ResourceManager::LoadShader("waterdemo-phong", "examples/water/shaders/default.vert.glsl", "examples/water/shaders/phong.frag.glsl");
    }

    static void LoadResources() {
        WaterDemo::Skybox::LoadResources();
        WaterDemo::Grid::LoadResources();
        WaterDemo::Ball::LoadResources();
        WaterDemo::Dragon::LoadResources();
        WaterDemo::Torus::LoadResources();

        Loader::LoadResources();
    }

    void render() override {
        lightball.transform.Rotate(glm::angleAxis(glm::radians(30.0f) * Blib::getDeltaTime(), glm::vec3(0, 1, 0)));
        dragon.transform.RotateLocal(glm::angleAxis(glm::radians(-30.0f) * Blib::getDeltaTime(), glm::vec3(0, 1, 0)));
        torus.transform.RotateLocal(glm::angleAxis(glm::radians(-30.0f) * Blib::getDeltaTime(), glm::vec3(1, 0, 0)));

        phong.Use();
        phong.SetVec3("viewPos", Blib::camera.transform.getPosition());
        phong.SetVec3("lightPos", lightball.transform.getPosition());
        phong.SetVec3("objectColor", glm::vec3(1 ,1, 1));
        phong.SetVec3("lightColor", glm::vec3(1, 1, 1));

        screen.bindReflection();
        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Blib::camera.toggleReflect();
        {
            Blib::ResourceManager::GetShader("waterdemo-light").SetBool("reflectionMode", true);
            {
                lightball.render();
            }
            Blib::ResourceManager::GetShader("waterdemo-light").SetBool("reflectionMode", false);

            phong.Use();
            phong.SetBool("reflectionMode", true);
            {
                if (fresnel_test) {
                    for (int i = 0; i < 40; i++) {
                        torus.transform.Translate({0, 0, -5});
                        torus.render(phong);
                    }
                    torus.transform.Translate({0, 0, 200});
                } else {
                    l.renderSavedMeshes(dragon.transform.getModelMatrix(), true);
                    // dragon.render(phong);
                }
            }
            phong.SetBool("reflectionMode", false);


            skybox.render();
        }
        Blib::camera.toggleReflect();

        screen.bindRefraction();
        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            lightball.render();
            phong.Use();

            if (fresnel_test) {
                for (int i = 0; i < 40; i++) {
                    torus.transform.Translate({0, 0, -5});
                    torus.render(phong);
                }
                torus.transform.Translate({0, 0, 200});
            } else {
                l.renderSavedMeshes(dragon.transform.getModelMatrix());
                // dragon.render(phong);
            }

            skybox.render();
        }

        screen.unbind();
        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            lightball.render();
            phong.Use();

            if (fresnel_test) {
                for (int i = 0; i < 40; i++) {
                    torus.transform.Translate({0, 0, -5});
                    torus.render(phong);
                }
                torus.transform.Translate({0, 0, 200});
            } else {
                l.renderSavedMeshes(dragon.transform.getModelMatrix());
                // dragon.render(phong);
            }

            skybox.render();

            screen.bindTextureForProgram(Blib::ResourceManager::GetShader("waterdemo-water"));
            water.render(lightball.transform.getPosition());
        }

    }

    void renderUI() override {
        ImGui::Begin("Config");
        water.renderUI();
        ImGui::Checkbox("Fresnel Test", &fresnel_test);

        ImGui::SeparatorText("Objects");

        Blib::camera.RenderUI();
        if (ImGui::TreeNode("Dragon")) {
            dragon.transform.RenderUI("dragon");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Torus")) {
            torus.transform.RenderUI("Torus");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Light Source")) {
            lightball.transform.RenderUI("Light Source");
            ImGui::TreePop();
        }
        ImGui::End();
    }
};
