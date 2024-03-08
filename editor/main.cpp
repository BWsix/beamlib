#include <beamlib.h>

#include "cube.hpp"

int main() {
    const auto window = beamlib::CreateWindow("beamlib");

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    beamlib::camera.transform.Translate({0, 0, 10});

    Cube cube;
    CubeInstance root(&cube, "root"), child1(&cube, "child1"), child2(&cube, "child2");
    root.color = {1, 0, 0};
    root.transform.Translate({1, 0, 0});

    child1.color = {1, 1, 0};
    child1.transform.Translate({1, 0, 0});
    root.PushChild(&child1);

    child2.color = {0, 1, 1};
    child2.transform.Translate({1, 0, 0});
    child1.PushChild(&child2);

    while (!beamlib::WindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        root.Update();
        root.Render();

        beamlib::BeginUI();
        root.RenderUI();
        beamlib::EndUI();
    }

    beamlib::DestroyWindow(window);

    return 0;
}
