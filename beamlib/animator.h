#pragma once

#include "utils.h"
#include "instance.h"

class Animator {
    float startTime = 0.0f;
    float progress = 0.0f;

    bool playing = false;
    float pauseStartTime = 0.0f;

    float frameStartTime = 0.0f;
    int frameIdx = 0;

public:
    float looping = false;
    std::vector<json> frames;

    bool isPlaying() { return playing; }

    void Pause() {
        playing = false;
        pauseStartTime = Blib::getTimeElapsed();
    }
    void Resume() {
        playing = true;
        frameStartTime += Blib::getTimeElapsed() - pauseStartTime;
    }

    void Update(Blib::Instance *root) {
        if (!playing) return;
        if (!looping && (frameIdx % frames.size()) == frames.size() - 1) {
            End();
            return;
        }

        float deltaTime = Blib::getTimeElapsed() - frameStartTime;
        if (deltaTime < 0.5f) {
            progress = deltaTime / 0.5f;
        } else {
            frameStartTime = Blib::getTimeElapsed();
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
        startTime = Blib::getTimeElapsed();
        frameStartTime = Blib::getTimeElapsed();
        playing = true;
        progress = 0.0f;
        frameIdx = 0;
    }

    void End() {
        playing = false;
        progress = 1.0f;
    }
};
