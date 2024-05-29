#pragma once

#include <beamlib.h>
#include "ball.h"
#include "depthmap.h"
#include "gumdam.h"
#include "screen.h"
#include "skybox.h"
#include "water.h"
#include "water_screen.h"

struct Scene : Blib::Scene {
    struct {
        glm::vec3 lightColor = glm::vec3(1.0);
        bool rotateLightSource = false;
    } conf;

    Depthmap depthmap;
    Screen screen = Screen(Blib::WIDTH, Blib::HEIGHT);
    Skybox skybox;
    Gumdam gumdam;
    Water water;
    Ball lightball;
    WaterScreen water_screen = WaterScreen(Blib::WIDTH, Blib::HEIGHT);

    Blib::ShaderProgram screenProgram;
    Blib::ShaderProgram gaussianBlurProgram;
    Blib::ShaderProgram waterScreenProgram;

    void setup() override {
        glClearColor(0, 0, 0, 1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POINT_SPRITE);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_CULL_FACE);

        gumdam.setup();
    }

    void init() override {
        gumdam.root.transform.Translate({0.0, 21.2, 0.0});
        water.transform.Scale({15, 1, 15});
        water.transform.Translate({0, -1, 0});
        lightball.transform.Translate({-15, 50, 15});

        screenProgram.Compile("shaders/screen.vert.glsl", "shaders/screen.frag.glsl");
        gaussianBlurProgram.Compile("shaders/screen.vert.glsl", "shaders/gaussianBlur.frag.glsl");
        waterScreenProgram.Compile("shaders/screen.vert.glsl", "examples/water/shaders/screen.frag.glsl");

        water_screen.loadResources();
        screen.loadResources();
        depthmap.loadResources();

        static Blib::Instance scene("DEFAULT SCENE");
        scene.PushChild(&Blib::camera);
        scene.PushChild(&gumdam.root);

        gumdam.init();
    }
    
    static void LoadResources() {
        Gumdam::LoadResources();
        Water::LoadResources();
        Skybox::LoadResources();
        Ball::LoadResources();
    }

    void render() override {
        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (ImGui::IsKeyPressed(ImGuiKey_T)) devMode = !devMode;
        }

        gumdam.update();

        if (conf.rotateLightSource) {
            float angle = Blib::getDeltaTime() * 0.3;
            lightball.transform.Rotate(glm::angleAxis(angle, glm::vec3(0, 1, 0)));
        }
        auto lightPos = lightball.transform.getPosition();

        // generating shadow map
        {
            depthmap.bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            auto prog = depthmap.getProg();
            prog.Use();
            prog.SetMat4("vp", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            gumdam.render(prog, lightPos);

            prog = Blib::ResourceManager::GetShader("water");
            prog.SetMat4("vp", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            water.render(lightPos);
        }

        {
            water_screen.bindReflection();
            glClearColor(0.0, 0.0, 0.0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Blib::camera.toggleReflect();

            auto prog = Blib::ResourceManager::GetShader("gumdam");
            prog.SetBool("toon", screen.toon);
            prog.SetMat4("lightVP", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            prog.SetInt("depthTex", 10);
            prog.SetBool("reflectionMode", true);
            gumdam.render(prog, lightPos);
            prog.SetBool("reflectionMode", false);

            skybox.render();

            Blib::camera.toggleReflect();
        }

        {
            water_screen.bindRefraction();
            glClearColor(0.0, 0.0, 0.0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            lightball.render(glm::vec3(1));

            auto prog = Blib::ResourceManager::GetShader("gumdam");
            prog.SetBool("toon", screen.toon);
            prog.SetMat4("lightVP", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            prog.SetInt("depthTex", 10);
            gumdam.render(prog, lightPos);

            skybox.render();
        }

        {
            screen.bind();
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE10);
            glBindTexture(GL_TEXTURE_2D, depthmap.texture_depth);

            auto prog = Blib::ResourceManager::GetShader("gumdam");
            prog.SetMat4("lightVP", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            prog.SetInt("depthTex", 10);
            prog.SetBool("toon", screen.toon);
            gumdam.render(prog, lightPos, true);

            if (gumdam.particle_on) {
                gumdam.kame_particle.render(Blib::ResourceManager::GetShader("gumdam-particles"));
            }

            if (gumdam.explosion_on) {
                gumdam.explosion.render(Blib::ResourceManager::GetShader("gumdam-explosion"), glm::mat4(1));
            }

            screen.blur(gaussianBlurProgram);

            screen.bind();

            skybox.render();
            lightball.render(conf.lightColor);

            // prog = Blib::ResourceManager::GetShader("floor");
            // prog.Use();
            // prog.SetMat4("lightVP", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            // prog.SetInt("depthTex", 10);
            // grid.renderFloor(prog);

            prog = Blib::ResourceManager::GetShader("water");
            prog.SetMat4("lightVP", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            prog.SetInt("depthTex", 10);
            water_screen.bindTextureForProgram(prog);
            water.render(lightball.transform.getPosition());

            screen.unbind();
        }

        // full screen quad
        {
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            screen.render(screenProgram);
        }
    }

    bool devMode;
    void renderUI() override {
        gumdam.renderControlPanel();
        if (devMode) {
            ImGui::Begin("Objects");
            {
                gumdam.renderUI();

                if (ImGui::TreeNode("light ball")) {
                    lightball.transform.RenderUI("Light Source");
                    ImGui::ColorEdit3("light Color", &conf.lightColor[0]);
                    ImGui::TreePop();
                }
                Blib::camera.RenderUI();
            }
            ImGui::End();
        }

        ImGui::Begin("Effects", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::SeparatorText("Camera");
            ImGui::Checkbox("Disable Camera Animator", &gumdam.cameraAnimator.disabled);

            ImGui::SeparatorText("Shadow");
            ImGui::Checkbox("Rotate Light Source", &conf.rotateLightSource);

            screen.renderUI();
            water.renderUI();

            ImGui::SeparatorText("Instanced rendering");
            ImGui::SliderInt("Instance Count", &gumdam.count, 1, 64);
        }
        ImGui::End();
    }
};
