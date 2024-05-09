#include <beamlib.h>
#include "models.h"
#include "skybox.h"

int main() {
    const auto window = Blib::CreateWindow("envmap", Blib::WIDTH, Blib::HEIGHT);
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height) {
        Blib::WIDTH = width;
        Blib::HEIGHT = height;
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
    });

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    Cybertruck::LoadResources();
    Cybertruck cybertruck("CyberTruck", Blib::ResourceManager::GetModel("cybertruck"));
    cybertruck.transform.Scale({10, 10, 10});

    Blib::camera.setMovementSpeed(20);
    Blib::camera.transform.Translate({-38.276, 5.635, -13.720});
    Blib::camera.setPitch(-3.150);
    Blib::camera.setYaw(-338.049);

    Skybox::LoadResources();
    Skybox skybox;

    Blib::ShaderProgram phong = Blib::ResourceManager::LoadShader("phong", "examples/envmap/shaders/default.vert.glsl", "examples/envmap/shaders/phong.frag.glsl");

    while (!Blib::WindowShouldClose(window)) {
        Blib::mouse.Update(window);
        Blib::camera.Update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);
        auto& prog = phong;
        prog.Use();
        prog.SetVec3("viewPos", Blib::camera.transform.getPosition());
        prog.SetVec3("lightPos", Blib::camera.transform.getPosition());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        cybertruck.transform.RotateLocal({0, Blib::getDeltaTime(), 0});
        cybertruck.Render(prog);

        skybox.render();

        Blib::BeginUI();
        ImGui::Begin("Config");
        cybertruck.RenderUI();
        Blib::camera.RenderUI();
        ImGui::End();
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
