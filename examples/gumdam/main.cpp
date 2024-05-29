#include <beamlib.h>
#include "screen.h"
#include "scene.h"

int main() {
    const auto window = Blib::CreateWindow("gumdam", Blib::WIDTH, Blib::HEIGHT);

    Scene::LoadResources();
    static Scene defaultScene;
    defaultScene.init();
    defaultScene.setup();

    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::WIDTH = width;
        Blib::HEIGHT = height;
        Blib::camera.setAspect((float)Blib::WIDTH / Blib::HEIGHT);
        defaultScene.screen.resize(width, height);
        defaultScene.water_screen.resize(width, height);
    });

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        Blib::mouse.Update(window);
        Blib::camera.Update();

        defaultScene.render();

        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (ImGui::IsKeyPressed(ImGuiKey_T)) defaultScene.devMode = !defaultScene.devMode;
        }

        Blib::BeginUI();
        {
            ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            {
                ImGui::Text("fps: %.0f", Blib::getFrameRate());
                ImGui::Text("frametime: %f", Blib::getDeltaTime());
                ImGui::Text("dev mode: %s (press t to toggle)", defaultScene.devMode ? "on" : "off");
            }
            ImGui::End();

            defaultScene.renderUI();
        }
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
