#include <beamlib.h>

#include "gumdam/scene.h"
#include "toon/scene.h"
#include "water/scene.h"
#include "envmap/scene.h"

int main() {
    const auto window = Blib::CreateWindow("hub", Blib::WIDTH, Blib::HEIGHT);

    Scene::LoadResources();
    static Scene gumdamScene;
    gumdamScene.init();
    gumdamScene.setup();

    SceneToon::LoadResources();
    static SceneToon toonScene;
    toonScene.init();
    toonScene.setup();

    WaterScene::LoadResources();
    static WaterScene waterScene;
    waterScene.init();
    waterScene.setup();

    EnvMapScene::LoadResources();
    static EnvMapScene envMapScene;
    envMapScene.init();
    envMapScene.setup();

    gumdamScene.setup();

    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::WIDTH = width;
        Blib::HEIGHT = height;
        Blib::camera.setAspect((float)Blib::WIDTH / Blib::HEIGHT);
        gumdamScene.screen.resize(width, height);
        gumdamScene.water_screen.resize(width, height);
        toonScene.screen.resize(width, height);
        waterScene.screen.resize(width, height);
    });

    Blib::Scene* active_scene = &gumdamScene;

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        Blib::mouse.Update(window);
        Blib::camera.Update();

        active_scene->render();

        Blib::BeginUI();
        ImGui::ShowMetricsWindow();

        active_scene->renderUI();

        ImGui::Begin("Scene Manager");
        if (ImGui::Button("Gumdam")) {
            active_scene = &gumdamScene;
            active_scene->setup();
        }
        if (ImGui::Button("Toon")) {
            active_scene = &toonScene;
            active_scene->setup();
        }
        if (ImGui::Button("Water")) {
            active_scene = &waterScene;
            active_scene->setup();
        }
        if (ImGui::Button("Env Map")) {
            active_scene = &envMapScene;
            active_scene->setup();
        }
        ImGui::End();

        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
