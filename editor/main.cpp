#include <beamlib.h>

int main() {
    const auto window = Blib::CreateWindow("Blib");

    // setup resize callback
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
    });

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    Blib::camera.transform.Translate({0, 0, 10});

    Blib::ShaderProgram prog;
    prog.Load("shaders/gundam.vert.glsl", "shaders/gundam.frag.glsl");
    Blib::Model body;
    body.load("models/body.obj");
    Blib::Model head;
    head.load("models/head.obj");

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
        Blib::mouse.Update(window);
        Blib::camera.Update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog.Use();
        prog.setUniformMat4("model", glm::mat4(1));
        prog.setUniformMat4("view", Blib::camera.getViewMatrix());
        prog.setUniformMat4("projection", Blib::camera.getProjectionMatrix());
        body.draw(prog);
        prog.setUniformMat4("model", glm::translate(glm::mat4(1), glm::vec3(0.0f, 4.0f, 0.0f)));
        head.draw(prog);

        Blib::BeginUI();
        ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("fps: %.0f", Blib::getFrameRate());
        ImGui::Text("frametime: %f", Blib::getDeltaTime());
        ImGui::End();

        ImGui::Begin("Object Hierarchy");
        ImGui::End();
        Blib::EndUI();
    }

    Blib::DestroyWindow(window);

    return 0;
}
