#pragma once

#include "easings.h"
#include "frameData.h"
#include "imgui.h"
#include "utils.h"

namespace Blib {

struct Frame {
    json data;
    float duration = 0.0f;
    Blib::EaseFunc easingFunc;
    float absoluteDuration = 0.0f;

    Frame(json frame, float d = 0.0f): absoluteDuration(d) {
        data = frame["data"];
        duration = frame["duration"];
        easingFunc = frame["easingFunc"];
        absoluteDuration = frame["absoluteDuration"];
    }

    void setAbsoluteDuration(float d) {
        absoluteDuration = d;
    }

    void RenderUI(std::string name) {
        ImGui::DragFloat(label("duration##" + name), &duration, 0.05f, 0.05f, 10.0f);

        ImGui::SameLine();
        ImGui::Combo(label("easing func##" + name), (int *)&easingFunc, easeFuncNames.data(), easeFuncNames.size());
    }

    json Serialize() { 
        json j;
        j["data"] = data;
        j["duration"] = duration;
        j["easingFunc"] = easingFunc;
        j["absoluteDuration"] = absoluteDuration;
        return j;
    }
};

}
