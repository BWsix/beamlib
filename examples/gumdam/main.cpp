#include <beamlib.h>
#include "gumdam.h"
#include "grid.h"
#include "skybox.h"

bool devmode = false;

int main() {
    const auto window = Blib::CreateWindow("gundam");

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
    });

    Blib::camera.transform.Translate({0, 2, 14});

    Gundam::LoadResources();
    Gundam gumdam;
    gumdam.root.transform.Translate({0.0, 21.2, 0.0});
    gumdam.idle();

    Skybox::LoadResources();
    Skybox skybox;

    Grid::LoadResources();
    Grid grid(100);

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        Blib::mouse.Update(window);
        Blib::camera.Update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.render();
        grid.render();
        gumdam.render();

        Blib::BeginUI();
        {
            ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            {
                // ImGui::Text("fps: %.0f", glm::max<float>(120.0f, Blib::getFrameRate()));
                ImGui::Text("fps: %.0f", Blib::getFrameRate());
                ImGui::Text("frametime: %f", Blib::getDeltaTime());
            }
            ImGui::End();

            gumdam.renderControlPanel();
            if (devmode) {
                ImGui::Begin("Objects");
                {
                    gumdam.renderUI();
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
