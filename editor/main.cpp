#include <beamlib.h>

#include "model.h"

#include "shaderProgramStore.h"
#include "cube.hpp"
#include "star.hpp"
#include "grid.hpp"
#include "bot.hpp"

const char *frame_standing = R"json(
{"children":[{"children":[{"children":[],"data":null,"name":"botLegLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLegRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[-0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyLower","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[1.2000000476837158,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[-1.2000000476837158,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLid","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,2.950000047683716,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyUpper","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *frame_trashcan = R"json(
{"children":[{"children":[{"children":[],"data":null,"name":"botLegLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.4000000059604645,0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLegRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[-0.4000000059604645,0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyLower","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.20000000298023224,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.8500000238418579,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperRight","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[-0.8500000238418579,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLid","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,2.799999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyUpper","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *frame_left = R"json(
{"children":[{"children":[{"children":[],"data":null,"name":"botLegLeft","transform":{"localRotationQuat":[0.34289664030075073,0.0,0.0,0.9393696784973145],"position":[0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLegRight","transform":{"localRotationQuat":[-0.34289753437042236,0.0,0.0,0.9393720030784607],"position":[-0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyLower","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerLeft","transform":{"localRotationQuat":[-0.41231897473335266,0.0,0.0,0.9110347628593445],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,0.9999997615814209],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperLeft","transform":{"localRotationQuat":[-0.19866922497749329,0.0,0.0,0.9800659418106079],"position":[1.2000000476837158,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerRight","transform":{"localRotationQuat":[0.0,0.0,0.0,0.9999984502792358],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperRight","transform":{"localRotationQuat":[0.412320613861084,0.0,0.0,0.911038339138031],"position":[-1.2000000476837158,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLid","transform":{"localRotationQuat":[-0.04813026636838913,-0.27155229449272156,0.013468792662024498,0.9611251354217529],"position":[0.0,3.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyUpper","transform":{"localRotationQuat":[0.0,-0.07492992281913757,0.0,0.9971871376037598],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *frame_right = R"json(
{"children":[{"children":[{"children":[],"data":null,"name":"botLegLeft","transform":{"localRotationQuat":[-0.3428966999053955,0.0,0.0,0.9393696784973145],"position":[0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLegRight","transform":{"localRotationQuat":[0.34289753437042236,0.0,0.0,0.9393720030784607],"position":[-0.4000000059604645,0.25,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyLower","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerLeft","transform":{"localRotationQuat":[0.0,0.0,0.0,0.9999955892562866],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,0.9999997615814209],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperLeft","transform":{"localRotationQuat":[0.4123205244541168,0.0,0.0,0.9110382199287415],"position":[1.2000000476837158,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[{"children":[],"data":null,"name":"botArmLowerRight","transform":{"localRotationQuat":[-0.41232019662857056,0.0,0.0,0.9110373258590698],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botArmUpperRight","transform":{"localRotationQuat":[-0.19866926968097687,0.0,0.0,0.980066180229187],"position":[-1.2000000476837158,2.549999952316284,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}},{"children":[],"data":null,"name":"botLid","transform":{"localRotationQuat":[-0.048069052398204803,0.271083801984787,-0.013685651123523712,0.9612572193145752],"position":[0.0,3.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}],"data":null,"name":"botBodyUpper","transform":{"localRotationQuat":[-1.6874354136220404e-09,0.07492976635694504,2.245687014124087e-08,0.9971863627433777],"position":[0.0,0.0,0.0],"rotationQuat":[-6.3421390755340765e-16,-3.349859653153214e-23,5.2819029150441565e-08,0.999998927116394],"scale":[1.0,1.0,1.0]}}
)json";

class Animator {
    float startTime = 0.0f;
    float progress = 0.0f;

    bool playing = false;
    float pauseStartTime = 0.0f;

    float frameStartTime = 0.0f;
    int frameIdx = 0;

public:
    float looping = true;
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
        if (!looping && (frameIdx % frames.size()) == frames.size() - 1) {
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
    // animator.frames.push_back(json::parse(frame_standing));
    // animator.frames.push_back(json::parse(frame_trashcan));
    animator.frames.push_back(json::parse(frame_right));
    animator.frames.push_back(json::parse(frame_standing));
    animator.frames.push_back(json::parse(frame_left));
    animator.frames.push_back(json::parse(frame_standing));
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

    botBodyUpper.Load(json::parse(frame_left));
    while (!beamlib::WindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            animator.looping = !animator.looping;
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            if (animator.isPlaying()) {
                animator.Pause();
            } else {
                animator.Resume();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            if (!animator.isPlaying()) {
                animator.Play();
            }
        }

        animator.Update(&botBodyUpper);

        root.Update();
        root.Render();

        beamlib::BeginUI();

        ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("fps: %.0f", beamlib::getFrameRate());
        ImGui::Text("frametime: %f", beamlib::getDeltaTime());
        ImGui::Text("Looping: %s", animator.looping ? "Yes" : "No");
        ImGui::Text("Playing: %s", animator.isPlaying() ? "Yes" : "No");
        ImGui::End();

        ImGui::Begin("Object Hierarchy");
        root.RenderUI();
        ImGui::End();
        beamlib::EndUI();
    }

    beamlib::DestroyWindow(window);

    return 0;
}
