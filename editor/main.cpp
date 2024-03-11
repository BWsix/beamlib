#include <beamlib.h>

#include "shaderProgramStore.h"
#include "cube.hpp"
#include "star.hpp"
#include "grid.hpp"

const char *frame1 = R"json(
{"children":[{"children":[{"children":[{"children":[{"children":[],"data":{"color":[1.0,1.0,1.0]},"name":"cube4","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":{"color":[1.0,1.0,1.0]},"name":"cube3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":{"color":[1.0,1.0,1.0]},"name":"cube2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":{"color":[1.0,1.0,1.0]},"name":"cube1","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"Model Manager","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *frame2 = R"json(
{"children":[{"children":[{"children":[{"children":[{"children":[],"data":{"color":[0.8629999756813049,1.0,0.8899999856948853]},"name":"cube4","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[1.0,1.0,-1.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":{"color":[1.0,0.8980000019073486,0.7289999723434448]},"name":"cube3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[1.0,1.0,-1.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":{"color":[0.8088235259056091,0.6938437223434448,0.6938437223434448]},"name":"cube2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[1.0,1.0,-1.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":{"color":[1.0,0.7689999938011169,0.9100000262260437]},"name":"cube1","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"Model Manager","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

class Animator {
    float startTime = 0.0f;
    bool playing = false;
    float progress = 0.0f;

public:
    std::vector<json> frames;

    bool isPlaying() { return playing; }

    void Update(beamlib::Instance *root) {
        if (!playing) return;

        float deltaTime = beamlib::getTimeElapsed() - startTime;
        if (deltaTime < 1.0f) {
            progress = deltaTime / 1.0f;
        } else {
            End();
            return;
        }

        json frame = root->Interpolator(frames[0], frames[1], progress);
        root->Load(frame);
    }

    void Play() {
        startTime = beamlib::getTimeElapsed();
        playing = true;
        progress = 0.0f;
    }

    void End() {
        playing = false;
        progress = 1.0f;
    }
};

int main(int argc, char **argv) {
    const auto window = beamlib::CreateWindow("beamlib");
    store::Init();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    beamlib::camera.transform.Translate({0, 0, 10});

    Cube cube;
    Star star;

    beamlib::DummyInstance root(NULL, "Root");
    root.PushChild(&beamlib::camera);

    Grid gridObject;
    GridInstance gridXZ(&gridObject, "Grid XZ");
    root.PushChild(&gridXZ);

    beamlib::DummyInstance modelManager(NULL, "Model Manager");
    root.PushChild(&modelManager);

    CubeInstance cube1(&cube, "cube1");
    modelManager.PushChild(&cube1);
    CubeInstance cube2(&cube, "cube2");
    cube1.PushChild(&cube2);
    CubeInstance cube3(&cube, "cube3");
    cube2.PushChild(&cube3);
    CubeInstance cube4(&cube, "cube4");
    cube3.PushChild(&cube4);

    Animator animator;
    animator.frames.push_back(json::parse(frame1));
    animator.frames.push_back(json::parse(frame2));
    animator.Play();

    while (!beamlib::WindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto leftButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
        if (leftButtonStatus == GLFW_PRESS && !animator.isPlaying()) {
            animator.Play();
        }

        animator.Update(&modelManager);
        root.Update();
        root.Render();

        beamlib::BeginUI();

        ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("fps: %.0f", beamlib::getFrameRate());
        ImGui::Text("frametime: %f", beamlib::getDeltaTime());
        ImGui::End();

        ImGui::Begin("Object Hierarchy");
        root.RenderUI();
        ImGui::End();
        beamlib::EndUI();
    }

    beamlib::DestroyWindow(window);

    return 0;
}
