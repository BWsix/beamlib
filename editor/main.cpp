#include <beamlib.h>

void loadResources();

int main() {
    const auto window = Blib::CreateWindow("Blib");
    loadResources();

    // setup resize callback
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
    });

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    Blib::camera.transform.Translate({0, 0, 10});

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
        Blib::mouse.Update(window);
        Blib::camera.Update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void loadResources() {
    // gundam
    Blib::ResourceManager::LoadShader("prog", "shaders/gundam.vert.glsl", "shaders/gundam.frag.glsl");
    Blib::ResourceManager::LoadModel("back", "models/gundam/back.obj");
    Blib::ResourceManager::LoadModel("body", "models/gundam/body.obj");
    Blib::ResourceManager::LoadModel("dbody", "models/gundam/dbody.obj");
    Blib::ResourceManager::LoadModel("dlefthand", "models/gundam/dlefthand.obj");
    Blib::ResourceManager::LoadModel("dleftleg", "models/gundam/dleftleg.obj");
    Blib::ResourceManager::LoadModel("drighthand", "models/gundam/drighthand.obj");
    Blib::ResourceManager::LoadModel("drightleg", "models/gundam/drightleg.obj");
    Blib::ResourceManager::LoadModel("head", "models/gundam/head.obj");
    Blib::ResourceManager::LoadModel("leftfoot", "models/gundam/leftfoot.obj");
    Blib::ResourceManager::LoadModel("lefthand", "models/gundam/lefthand.obj");
    Blib::ResourceManager::LoadModel("lshouder", "models/gundam/lshouder.obj");
    Blib::ResourceManager::LoadModel("rightfoot", "models/gundam/rightfoot.obj");
    Blib::ResourceManager::LoadModel("righthand", "models/gundam/righthand.obj");
    Blib::ResourceManager::LoadModel("rshouder", "models/gundam/rshouder.obj");
    Blib::ResourceManager::LoadModel("ulefthand", "models/gundam/ulefthand.obj");
    Blib::ResourceManager::LoadModel("uleftleg", "models/gundam/uleftleg.obj");
    Blib::ResourceManager::LoadModel("urighthand", "models/gundam/urighthand.obj");
    Blib::ResourceManager::LoadModel("urightleg", "models/gundam/urightleg.obj");
}
