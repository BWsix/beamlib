#include "models.h"
#include "screen.h"
#include <beamlib.h>

struct {
    glm::vec3 objectColor = glm::vec3(1.0);
    glm::vec3 lightColor = glm::vec3(1.0);

    float toonThreshold = 0.7;
} config;

int main() {
    const auto window = Blib::CreateWindow("lighting", Blib::WIDTH, Blib::HEIGHT);
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height) {
        Blib::WIDTH = width;
        Blib::HEIGHT = height;
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
    });

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    Blib::camera.setMovementSpeed(2);
    Blib::camera.transform.Translate({0.0, 2.0, 5.0});
    Blib::camera.setPitch(-26.1);

    Ball::LoadResources();
    Ball lightball;
    lightball.transform.Scale({0.1, 0.1, 0.1});
    lightball.transform.Translate({1.0, 3.0, -3.0});

    Dragon::LoadResources();
    Dragon dragon;
    dragon.transform.Scale({5, 5, 5});

    Screen::LoadResources();
    Screen screen;

    struct {
        Blib::ShaderProgram phong = Blib::ResourceManager::LoadShader("phong", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/phong.frag.glsl");
        Blib::ShaderProgram toon = Blib::ResourceManager::LoadShader("toon", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/toon.frag.glsl");
        Blib::ShaderProgram light = Blib::ResourceManager::LoadShader("light", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/light.frag.glsl");
        Blib::ShaderProgram screen = Blib::ResourceManager::GetShader("screen");
    } programs;

    while (!Blib::WindowShouldClose(window)) {
        Blib::mouse.Update(window);
        Blib::camera.Update();

        dragon.transform.RotateLocal({0.0, Blib::getDeltaTime(), 0.0});

        screen.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8, 0.4, 0.4, 1);

        auto& prog = programs.toon;
        prog.Use();
        prog.SetVec3("viewPos", Blib::camera.transform.getPosition());
        // prog.SetVec3("lightPos", lightball.transform.getPosition());
        prog.SetVec3("lightPos", Blib::camera.transform.getPosition());
        prog.SetVec3("objectColor", config.objectColor);
        prog.SetVec3("lightColor", config.lightColor);
        dragon.render(prog);

        // programs.light.Use();
        // programs.light.SetVec3("lightColor", config.lightColor);
        // lightball.render(programs.light);

        screen.unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        programs.screen.Use();
        programs.screen.SetFloat("threshold", config.toonThreshold);
        screen.render(programs.screen);

        Blib::BeginUI();
        ImGui::Begin("Config");

        ImGui::ColorEdit3("objectColor", &config.objectColor[0]);
        ImGui::ColorEdit3("lightColor", &config.lightColor[0]);
        ImGui::DragFloat("toonThreshold", &config.toonThreshold, 0.01, 0, 1);

        Blib::camera.RenderUI();
        if (ImGui::TreeNode("Light Source")) {
            lightball.transform.RenderUI("Light Source");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Object")) {
            dragon.transform.RenderUI("Object");
            ImGui::TreePop();
        }

        ImGui::End();
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
