#include <beamlib.h>

#include "shaderProgramStore.h"
#include "cube.hpp"
#include "star.hpp"

int main(int argc, char **argv) {
    const auto window = beamlib::CreateWindow("beamlib");
    store::Init();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    beamlib::camera.transform.Translate({0, 0, 10});

    Cube cube;
    Star star;

    beamlib::DummyInstance root(NULL, "ROOT");
    root.PushChild(&beamlib::camera);

    CubeInstance wrist(&cube, "wrist");
    root.PushChild(&wrist);
    CubeInstance palm(&cube, "palm");
    wrist.PushChild(&palm);
    CubeInstance thumb_root(&cube, "thumb_root");
    palm.PushChild(&thumb_root);
    CubeInstance thumb_tip(&cube, "thumb_tip");
    thumb_root.PushChild(&thumb_tip);

    if (argc == 2) {
        root.Load(json::parse(beamlib::SlurpFile(argv[1])));
    }

    while (!beamlib::WindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        root.Update();
        root.Render();

        beamlib::BeginUI();

        ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("fps: %.0f", beamlib::getFrameRate());
        ImGui::Text("frametime: %f", beamlib::getDeltaTime());
        ImGui::End();

        ImGui::Begin("Object hierarchy");
        root.RenderUI();
        ImGui::End();
        beamlib::EndUI();
    }

    beamlib::DestroyWindow(window);

    return 0;
}
