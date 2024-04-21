#include <beamlib.h>
#include "gumdam.h"
#include "grid.h"
#include "skybox.h"
#include "lilypad.h"

bool devMode = false;

int main() {
    const auto window = Blib::CreateWindow("gundam");

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Blib::Instance scene("DEFAULT SCENE");
    scene.PushChild(&Blib::camera);

    Gundam::LoadResources();
    Gundam gumdam;
    scene.PushChild(&gumdam.root);
    gumdam.root.transform.Translate({0.0, 21.2, 0.0});
    gumdam.init();

    Skybox::LoadResources();
    Skybox skybox;

    Lilypad::LoadResources();
    Lilypad lilypad;
    lilypad.root.transform.Scale({1.0, 40.0, 40.0});
    lilypad.root.transform.RotateLocal({0.0, 0.0, 1.571});
    lilypad.root.transform.Translate({0.55, 0.1, 40});

    // Grid::LoadResources();
    // Grid grid(100);

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        Blib::mouse.Update(window);
        Blib::camera.Update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.render();
        // grid.render();
        gumdam.render();
        lilypad.render();

        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (ImGui::IsKeyPressed(ImGuiKey_T)) devMode = !devMode;
        }

        Blib::BeginUI();
        {
            ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            {
                ImGui::Text("fps: %.0f", Blib::getFrameRate());
                ImGui::Text("frametime: %f", Blib::getDeltaTime());
                ImGui::Text("dev mode: %s (press t to toggle)", devMode ? "on" : "off");
            }
            ImGui::End();

            gumdam.renderControlPanel();
            if (devMode) {
                ImGui::Begin("Objects");
                {
                    gumdam.renderUI();
                    lilypad.renderUI();
                    Blib::camera.RenderUI();
                }
                ImGui::End();
            }
        }
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
