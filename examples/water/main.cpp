#include "models.h"
#include "screen.h"
#include "skybox.h"
#include <beamlib.h>

int main() {
    const auto window = Blib::CreateWindow("water", Blib::WIDTH, Blib::HEIGHT);
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height) {
        Blib::WIDTH = width;
        Blib::HEIGHT = height;
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
    });

    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);

    Blib::camera.transform.Translate({0.0, 1.0, 20.0});

    Skybox::LoadResources();
    Skybox skybox;

    Grid::LoadResources();
    Grid water;
    water.transform.Scale({5, 1, 5});
    water.transform.Translate({0, -1, 0});

    Ball::LoadResources();
    Ball lightball;
    lightball.transform.Translate({5, 5, -5});

    Dragon::LoadResources();
    Dragon dragon;
    dragon.transform.Scale({5, 5, 5});
    dragon.transform.Translate({0, 1, 0});

    Torus::LoadResources();
    Torus torus;
    torus.transform.Translate({0, -10, 30});
    torus.transform.RotateLocal(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)));

    Screen screen(Blib::WIDTH, Blib::HEIGHT);
    screen.loadResources();

    Blib::ShaderProgram screenProgram;
    screenProgram.Compile("shaders/screen.vert.glsl", "examples/water/shaders/screen.frag.glsl");

    auto phong = Blib::ResourceManager::LoadShader("phong", "examples/water/shaders/default.vert.glsl", "examples/water/shaders/phong.frag.glsl");

    bool fresnel_test = false;

    while (!Blib::WindowShouldClose(window)) {
        Blib::mouse.Update(window);
        Blib::camera.Update();

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
            Blib::ResourceManager::GetShader("light").SetBool("reflectionMode", true);
            {
                lightball.render();
            }
            Blib::ResourceManager::GetShader("light").SetBool("reflectionMode", false);

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
                    dragon.render(phong);
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
                dragon.render(phong);
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
                dragon.render(phong);
            }

            skybox.render();

            screen.bindTextureForProgram(Blib::ResourceManager::GetShader("water"));
            water.render(lightball.transform.getPosition());
        }

        Blib::BeginUI();
        ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::Text("fps: %.0f", Blib::getFrameRate());
            ImGui::Text("frametime: %f", Blib::getDeltaTime());
        }
        ImGui::End();

        ImGui::Begin("Config");
        {
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
        }
        ImGui::End();
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
