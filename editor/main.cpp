#include <beamlib.h>
#include <glad/gl.h>
#include <imgui.h>

int main() {
    const auto window = beamlib::CreateWindow(800, 600, "beamlib");

    while (!beamlib::WindowShouldClose(window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        beamlib::BeginUI();
            ImGui::ShowDemoWindow();
        beamlib::EndUI();
    }

    beamlib::DestroyWindow(window);

    return 0;
}
