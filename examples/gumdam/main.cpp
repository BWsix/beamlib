#include <beamlib.h>
#include "ball.h"
#include "depthmap.h"
#include "grid.h"
#include "gumdam.h"
#include "screen.h"
#include "skybox.h"
#include "lilypad.h"

struct {
    bool devMode = false;

    glm::vec3 lightColor = glm::vec3(1.0);
    bool rotateLightSource = false;
} conf;

int main() {
    const auto window = Blib::CreateWindow("gumdam", Blib::WIDTH, Blib::HEIGHT);

    Blib::ShaderProgram screenProgram;
    screenProgram.Compile("shaders/screen.vert.glsl", "shaders/screen.frag.glsl");
    Blib::ShaderProgram gaussianBlurProgram;
    gaussianBlurProgram.Compile("shaders/screen.vert.glsl", "shaders/gaussianBlur.frag.glsl");
    static Screen screen(Blib::WIDTH, Blib::HEIGHT);
    screen.loadResources();

    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::WIDTH = width;
        Blib::HEIGHT = height;
        Blib::camera.setAspect((float)Blib::WIDTH / Blib::HEIGHT);
        screen.resize(width, height);
    });

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);  

    Blib::Instance scene("DEFAULT SCENE");
    scene.PushChild(&Blib::camera);

    Gumdam::LoadResources();
    Gumdam gumdam;
    scene.PushChild(&gumdam.root);
    gumdam.root.transform.Translate({0.0, 21.2, 0.0});
    gumdam.init();

    Skybox::LoadResources();
    Skybox skybox;

    Depthmap depthmap;
    depthmap.loadResources();

    Ball::LoadResources();
    Ball lightball;
    lightball.transform.Translate({-15, 50, 15});

    Grid::LoadResources();
    Grid grid;

    Lilypad::LoadResources();
    Lilypad lilypad;
    lilypad.root.transform.Scale({1.0, 40.0, 40.0});
    lilypad.root.transform.RotateLocal({0.0, 0.0, 1.571});
    lilypad.root.transform.Translate({0.55, 0.1, 40});

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        Blib::mouse.Update(window);
        Blib::camera.Update();

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

            prog.Use();
            prog.SetMat4("vp", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            grid.renderFloor(prog);
        }

        {
            screen.bind();
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE10);
            glBindTexture(GL_TEXTURE_2D, depthmap.texture_depth);

            auto prog = Blib::ResourceManager::GetShader("gumdam");
            prog.Use();
            prog.SetMat4("lightVP", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            prog.SetInt("depthTex", 10);
            gumdam.render(prog, lightPos);

            screen.blur(gaussianBlurProgram);

            screen.bind();
            skybox.render();
            lightball.render(conf.lightColor);

            prog = Blib::ResourceManager::GetShader("floor");
            prog.Use();
            prog.SetMat4("lightVP", depthmap.getLightProjection() * depthmap.getLightView(lightPos));
            prog.SetInt("depthTex", 10);
            grid.renderFloor(prog);
        }

        // full screen quad
        {
            screen.unbind();
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            screen.render(screenProgram);
        }

        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (ImGui::IsKeyPressed(ImGuiKey_T)) conf.devMode = !conf.devMode;
        }

        Blib::BeginUI();
        {
            ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            {
                ImGui::Text("fps: %.0f", Blib::getFrameRate());
                ImGui::Text("frametime: %f", Blib::getDeltaTime());
                ImGui::Text("dev mode: %s (press t to toggle)", conf.devMode ? "on" : "off");
            }
            ImGui::End();

            gumdam.renderControlPanel();
            if (conf.devMode) {
                ImGui::Begin("Objects");
                {
                    gumdam.renderUI();
                    lilypad.renderUI();

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
                // Camera
                ImGui::Checkbox("Disable Camera Animator", &gumdam.cameraAnimator.disabled);

                // Motion Blur
                ImGui::Checkbox("Motion Blur", &screen.motionBlur);
                // ImGui::SameLine();
                // ImGui::SliderInt("Blurness", &screen.blurness, 1, 100);

                // Pixelation
                ImGui::Checkbox("Pixelation", &screen.pixelation);
                ImGui::SameLine();
                ImGui::SliderFloat("Size", &screen.pixelSize, 1, 40);

                // HDR
                ImGui::SliderFloat("Exposure", &screen.exposure, 0.01, 10);

                // Shadow
                ImGui::Checkbox("Rotate Light Source", &conf.rotateLightSource);
            }
            ImGui::End();
        }
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
