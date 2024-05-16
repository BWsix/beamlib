#include <beamlib.h>
#include "gumdam.h"
#include "screen.h"
#include "skybox.h"
#include "lilypad.h"

struct {
    bool devMode = false;
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

    Blib::Instance scene("DEFAULT SCENE");
    scene.PushChild(&Blib::camera);

    Gumdam::LoadResources();
    Gumdam gumdam;
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

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        Blib::mouse.Update(window);
        Blib::camera.Update();

        screen.bind();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gumdam.render();
        screen.blur(gaussianBlurProgram);

        screen.bind();
        skybox.render();
        lilypad.render();

        screen.unbind();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screen.render(screenProgram);

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
            }
            ImGui::End();
        }
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
