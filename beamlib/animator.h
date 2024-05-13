#pragma once

#include "easings.h"
#include "frame.h"
#include "frameData.h"
#include "imgui.h"
#include "instance.h"
#include "utils.h"
#include <fstream>
#include <functional>
#include <beamlib.h>
#include <iterator>

namespace Blib {

struct FrameManager {
    float length = 0.0f;
    size_t frameCount = 0;
    std::map<float, Frame> absoluteTimedFrames;
    float currentFrameProgess = 0.0f;

    void loadFrames(std::vector<Frame>& frames) {
        absoluteTimedFrames.clear();
        frameCount = frames.size();

        float time = 0.0f;
        for (size_t i = 0; i < frameCount; ++i) {
            absoluteTimedFrames.insert({time, frames[i]});
            frames[i].setAbsoluteDuration(time);
            time += absoluteTimedFrames.at(time).duration;
        }
        length = time;
    }

    FrameData getFrame(float timeElaped, bool looping) {
        auto currentFrame = std::prev(absoluteTimedFrames.upper_bound(timeElaped));
        auto nextFrame = std::next(currentFrame);
        if (nextFrame == absoluteTimedFrames.end()) {
            if (looping) {
                nextFrame = absoluteTimedFrames.begin();
            } else {
                nextFrame = currentFrame;
            }
        }

        float frameStartTime = currentFrame->first;
        currentFrameProgess = (timeElaped - frameStartTime) / currentFrame->second.duration;
        auto idx = std::distance(absoluteTimedFrames.begin(), currentFrame);

        FrameData f = {
            idx,
            currentFrame->second.easingFunc,
            currentFrame->second.data,
            nextFrame->second.data,
            currentFrameProgess,
        };
        return f;
    }

    void Load(json j) {
        std::vector<Frame> frames;
        for (auto j : j["frames"]) {
            frames.push_back(Frame(j));
        }
    }

    json Serialize(std::vector<Frame>& frames) {
        json j;
        j["frames"] = json::array();
        for (auto f : frames) {
            j["frames"].push_back(f.Serialize());
        }
        return j;
    }
};

class Animator {
public:
    bool disabled = false;

    std::string name;
    float timeElapsed = 0.0f;

    bool playing = false;
    float pauseStartTime = 0.0f;

    float looping = false;
    FrameManager frameManager;
    std::vector<Frame> frames;
    size_t currentActiveFrameIdx = 0;
    Instance* instance;
    std::function<void()> animationEndingCallback;

    char save_as_filename[128] = "animations/output.json";

    Animator(std::string name, Instance* instance): name(name), instance(instance) {}

    void Pause() {
        playing = false;
        pauseStartTime = Blib::getTimeElapsed();
    }

    void Resume() {
        playing = true;
    }

    void renderUI() {
        // contoller
        if (ImGui::Button(label("Restart##ui" + name))) {
            timeElapsed = 0;
            frameManager.loadFrames(frames);
            Play(); 
        }

        ImGui::SameLine();
        if (ImGui::Button(label("Play##ui" + name))) {
            frameManager.loadFrames(frames);
            Resume(); 
        }

        ImGui::SameLine();
        if (ImGui::Button(label("Pause##ui" + name))) {
            Pause();
        }

        ImGui::SameLine();
        if (ImGui::Button(label("New Frame##ui" + name))) {
            json j;
            j[instance->name] = instance->Serialize();
            frames.push_back(Frame({
                {"data", j},
                {"duration", 1.0},
                {"easingFunc", EaseFunc::Lerp},
                {"absoluteDuration", 0.0},
            }));
            frameManager.loadFrames(frames);
        }

        ImGui::SameLine();
        if (ImGui::Button(label("Save As##ui" + name))) {
            Save();
        }

        ImGui::SameLine();
        ImGui::InputText(label("##outputfilename-ui" + name), save_as_filename, sizeof(save_as_filename));
        ImGui::SeparatorText("");

        for (size_t i = 0; i < frames.size(); ++i) {
            if (i == currentActiveFrameIdx) {
                ImGui::SeparatorText("v Current Active Frame v");
            }

            if (ImGui::Button(label("Frame " + std::to_string(i) + " ##ui" + name))) {
                currentActiveFrameIdx = i;
                timeElapsed = frames[i].absoluteDuration;
                Load(frames[i]);
            }

            ImGui::SameLine();
            if (ImGui::Button(label("Play##ui" + std::to_string(i) + name))) {
                currentActiveFrameIdx = i;
                timeElapsed = frames[i].absoluteDuration;
                Resume(); 
            }

            ImGui::SameLine();
            if (ImGui::Button(label("Delete##ui" + std::to_string(i) + name))) {
                frames.erase(frames.begin() + i);
                frameManager.loadFrames(frames);
            }

            if (i == currentActiveFrameIdx) {
                ImGui::SameLine();
                if (ImGui::Button(label("Save##ui" + std::to_string(i) + name))) {
                    frames[i].data[instance->name] = instance->Serialize();
                    frameManager.loadFrames(frames);
                }

                ImGui::SameLine();
                if (ImGui::Button(label(" ^ Duplicate##ui" + std::to_string(i) + name))) {
                    json j;
                    j[instance->name] = instance->Serialize();
                    frames.insert(frames.begin() + i, Frame({ {"data", j}, {"duration", frames[i].duration}, {"easingFunc", frames[i].easingFunc}, {"absoluteDuration", 0.0} }));
                    frameManager.loadFrames(frames);
                    currentActiveFrameIdx += 1;
                }

                ImGui::SameLine();
                if (ImGui::Button(label(" v Duplicate##ui" + std::to_string(i) + name))) {
                    json j;
                    j[instance->name] = instance->Serialize();
                    frames.insert(frames.begin() + i + 1, Frame({ {"data", j}, {"duration", frames[i].duration}, {"easingFunc", frames[i].easingFunc}, {"absoluteDuration", 0.0} }));
                    frameManager.loadFrames(frames);
                }

                if (i != 0) {
                    ImGui::SameLine();
                    if (ImGui::Button(label(" ^ Move##ui" + std::to_string(i) + name))) {
                        frames.insert(frames.begin() + i - 1, *(frames.begin() + i));
                        frames.erase(frames.begin() + i + 1);
                        currentActiveFrameIdx -= 1;
                        frameManager.loadFrames(frames);
                    }
                }

                if (i != frames.size() - 1) {
                    ImGui::SameLine();
                    if (ImGui::Button(label(" v Move##ui" + std::to_string(i) + name))) {
                        frames.insert(frames.begin() + i + 2, *(frames.begin() + i));
                        frames.erase(frames.begin() + i);
                        currentActiveFrameIdx += 1;
                        frameManager.loadFrames(frames);
                    }
                }

                frames[i].RenderUI(name + "frame" + std::to_string(i));
            }

            if (i == currentActiveFrameIdx) {
                char buf[32];
                sprintf(buf, "Progress: %f%%", frameManager.currentFrameProgess);
                ImGui::ProgressBar(frameManager.currentFrameProgess, {0, 0}, buf);

                ImGui::SeparatorText("^ Current Active Frame ^");
            }
        }
    }

    void Load(const Frame& f) {
        instance->Load(f.data[instance->name]);
    }

    void LoadJson(const json& j) {
        if (disabled) return;
        frames.clear();
        for (auto frame : j["frames"]) {
            frames.push_back({frame});
        }
        frameManager.loadFrames(frames);
        Load(frames[0]);
    }

    void Update() {
        if (disabled) return;
        if (!playing) return;
        if (timeElapsed >= frameManager.length) {
            if (looping) {
                timeElapsed = 0;
                return;
            }
            End();
            return;
        }

        auto framedata = frameManager.getFrame(timeElapsed, looping);
        currentActiveFrameIdx = framedata.idx;

        json frame = instance->Interpolator(framedata.from[instance->name], framedata.to[instance->name], ease(framedata.func, framedata.progress));
        instance->Load(frame);

        timeElapsed += getDeltaTime();
    }

    void Save() {
        std::ofstream output(save_as_filename);
        json j;
        j["frames"] = json::array();
        for (auto frame : frames) {
            j["frames"].push_back(frame.Serialize());
        }
        output << j;
        output.close();
    }

    void Play(std::function<void()> cb = [](){}) {
        if (disabled) return;
        timeElapsed = 0.0f;
        playing = true;
        animationEndingCallback = cb;
    }

    void End() {
        if (disabled) return;
        playing = false;
        animationEndingCallback();
    }
};

}
