#include <beamlib.h>
#include "scene.h"

int main() {
    const auto window = Blib::CreateWindow("water", Blib::WIDTH, Blib::HEIGHT);

    WaterScene::LoadResources();
    static WaterScene scene;
    scene.init();
    scene.setup();

    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height) {
        Blib::WIDTH = width;
        Blib::HEIGHT = height;
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
        scene.screen.resize(width, height);
    });

    while (!Blib::WindowShouldClose(window)) {
        Blib::mouse.Update(window);
        Blib::camera.Update();

        scene.render();

        Blib::BeginUI();
        ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::Text("fps: %.0f", Blib::getFrameRate());
            ImGui::Text("frametime: %f", Blib::getDeltaTime());
        }
        ImGui::End();

        scene.renderUI();
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
