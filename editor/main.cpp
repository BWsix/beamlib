#include <beamlib.h>

#include "model.h"

#include "shaderProgramStore.h"
#include "cube.hpp"
#include "star.hpp"
#include "grid.hpp"
#include "bot.hpp"

const char *frame_center = R"json(
    {"children":[{"children":[{"children":[{"children":[],"data":null,"name":"botLegLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLegRight","transform":{"localRotationQuat":[1.862645149230957e-09,0.0,0.0,0.9999942183494568],"position":[-0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyLower","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,0.9999991655349731],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,0.999999463558197],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[1.2000000476837158,2.5,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[-1.2000000476837158,2.5,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLid","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,3.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyUpper","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"Bot Manager","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *frame_left = R"json(
{"children":[{"children":[{"children":[{"children":[],"data":null,"name":"botLegLeft","transform":{"localRotationQuat":[-0.20169998705387115,0.0,0.0,0.9794461727142334],"position":[0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLegRight","transform":{"localRotationQuat":[0.2674925923347473,0.0,0.0,0.9635543823242188],"position":[-0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyLower","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerLeft","transform":{"localRotationQuat":[-0.04511643573641777,0.0,0.0,0.9989790320396423],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,0.9999994039535522],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[1.2000000476837158,2.5,0.0],"rotationQuat":[0.0,0.13498243689537048,0.0,0.9908475875854492],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerRight","transform":{"localRotationQuat":[-0.5442534685134888,0.0,0.0,0.8389189839363098],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[-1.2000000476837158,2.5,0.0],"rotationQuat":[0.0,0.15730559825897217,0.0,0.9875497817993164],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLid","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,3.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyUpper","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.02256408892571926,0.0,0.9997446537017822],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"Bot Manager","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *frame_right = R"json(
{"children":[{"children":[{"children":[{"children":[],"data":null,"name":"botLegLeft","transform":{"localRotationQuat":[0.1929464042186737,0.0,0.0,0.9812081456184387],"position":[0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLegRight","transform":{"localRotationQuat":[-0.25558239221572876,0.0,0.0,0.9667820334434509],"position":[-0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyLower","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,0.9999983906745911],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerLeft","transform":{"localRotationQuat":[-0.5358364582061768,0.0,0.0,0.8443182706832886],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,0.999999463558197],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[1.2000000476837158,2.5,0.0],"rotationQuat":[0.0,-0.1290816068649292,0.0,0.9916336536407471],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerRight","transform":{"localRotationQuat":[-0.042989570647478104,0.0,0.0,0.9990749359130859],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[-1.2000000476837158,2.5,0.0],"rotationQuat":[0.0,-0.15015698969364166,0.0,0.9886620044708252],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLid","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,3.000000238418579,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyUpper","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,-0.021716859191656113,0.0,0.9997636079788208],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"Bot Manager","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

class Animator {
    float startTime = 0.0f;
    float progress = 0.0f;
    float looping = true;

    bool playing = false;
    float pauseStartTime = 0.0f;

    float frameStartTime = 0.0f;
    int frameIdx = 0;

public:
    std::vector<json> frames;

    bool isPlaying() { return playing; }

    void Pause() {
        playing = false;
        pauseStartTime = beamlib::getTimeElapsed();
    }
    void Resume() {
        playing = true;
        frameStartTime += beamlib::getTimeElapsed() - pauseStartTime;
    }

    void Update(beamlib::Instance *root) {
        if (!playing) return;
        if (!looping && frameIdx == frames.size() - 1) {
            End();
            return;
        }

        float deltaTime = beamlib::getTimeElapsed() - frameStartTime;
        if (deltaTime < 0.5f) {
            progress = deltaTime / 0.5f;
        } else {
            frameStartTime = beamlib::getTimeElapsed();
            frameIdx += 1;
            return;
        }

        json frame = root->Interpolator(getFrame(frameIdx), getFrame(frameIdx + 1), progress);
        root->Load(frame);
    }

    json getFrame(int idx) {
        idx %= frames.size();
        return frames[idx];
    }

    void Play() {
        startTime = beamlib::getTimeElapsed();
        frameStartTime = beamlib::getTimeElapsed();
        playing = true;
        progress = 0.0f;
        frameIdx = 0;
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

    Animator animator;
    animator.frames.push_back(json::parse(frame_right));
    animator.frames.push_back(json::parse(frame_center));
    animator.frames.push_back(json::parse(frame_left));
    animator.frames.push_back(json::parse(frame_center));
    animator.Play();

    beamlib::DummyInstance botManager(NULL, "Bot Manager");
    root.PushChild(&botManager);

    BotBodyUpper botBodyUpperObject;
    BotBodyLower botBodyLowerObject;
    BotLid botLidObject;
    BotArmUpper botArmUpperObject;
    BotArmLower botArmLowerObject;
    BotLeg botLegObject;

    BotBodyUpperInstance botBodyUpper(&botBodyUpperObject, "botBodyUpper");
    botManager.PushChild(&botBodyUpper);
        BotBodyLowerInstance botBodyLower(&botBodyLowerObject, "botBodyLower");
        botBodyUpper.PushChild(&botBodyLower);

        BotArmUpperInstance botArmUpperLeft(&botArmUpperObject, "botArmUpperLeft");
        botBodyUpper.PushChild(&botArmUpperLeft);
            BotArmLowerInstance botArmLowerLeft(&botArmLowerObject, "botArmLowerLeft");
            botArmUpperLeft.PushChild(&botArmLowerLeft);

        BotArmUpperInstance botArmUpperRight(&botArmUpperObject, "botArmUpperRight");
        botBodyUpper.PushChild(&botArmUpperRight);
            BotArmLowerInstance botArmLowerRight(&botArmLowerObject, "botArmLowerRight");
            botArmUpperRight.PushChild(&botArmLowerRight);


        BotLegInstance botLegLeft(&botLegObject, "botLegLeft");
        botBodyLower.PushChild(&botLegLeft);

        BotLegInstance botLegRight(&botLegObject, "botLegRight");
        botBodyLower.PushChild(&botLegRight);

        BotLidInstance botLid(&botLidObject, "botLid");
        botBodyUpper.PushChild(&botLid);

    while (!beamlib::WindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto middleButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
        if (middleButtonStatus == GLFW_PRESS) {
            if (animator.isPlaying()) {
                animator.Pause();
            } else {
                animator.Resume();
            }
        }

        animator.Update(&botManager);

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
