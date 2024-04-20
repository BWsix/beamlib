#pragma once

#include "utils.h"
#include "instance.h"

namespace Blib {

class Camera : public Instance {
    float yaw = -90.0f;
    float pitch = 0.0f;
    float yaw_prev = -90.0f;
    float pitch_prev = 0.0f;

    Instance *targetInstance;
    float radius = 5;

    float fovy = 45;
    float aspect = 800.0 / 600;
    float near = 0.1;
    float far = 100;

    float sensitivity = 0.05;
    float movementSpeed = 10;

public:
    using Instance::Instance;

    void setAspect(float aspect) { this->aspect = aspect; }
    bool isFirstPersonMode() const {
        return targetInstance == NULL;
    }
    void setTargetInstance(Instance *target) {
        if (!targetInstance) {
            yaw_prev = yaw;
            pitch_prev = pitch;
        }
        targetInstance = target;
    }
    void removeTargetInstance() {
        yaw = yaw_prev;
        pitch = pitch_prev;
        setTargetInstance(NULL);
    }
    Transform *getTargetTranform() const { return &targetInstance->transform; }

    glm::vec3 getFront() const { return glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))); }
    glm::mat4 getViewMatrix() const {
        if (isFirstPersonMode()) {
            return glm::lookAt(transform.getLocalPosition(), transform.getLocalPosition() + getFront(), {0, 1, 0});
        } else {
            // TODO: this might not be the correct way of getting position, but it works so... yeah
            return glm::lookAt(getTargetTranform()->getPosition() - radius * getFront(), getTargetTranform()->getPosition(), {0, 1, 0});
        }
    }
    glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(fovy), aspect, near, far); }

    void Update() override;

    void CustomRenderUI() override {
        ImGui::DragFloat(("pitch##" + name).c_str(), &pitch, 0.1);
        ImGui::DragFloat(("yaw##" + name).c_str(), &yaw, 0.1);
        if (!isFirstPersonMode()) {
            ImGui::DragFloat(("radius##" + name).c_str(), &radius, 0.2);
            if (ImGui::Button(("Go Back To First Person Mode##" + name).c_str())) {
                removeTargetInstance();
            }
        }
    }

    json CustomSerialize() override {
        return {
            {"radius", radius},
            {"pitch", pitch},
            {"yaw", yaw},
            {"pitch_prev", pitch_prev},
            {"yaw_prev", yaw_prev},
            {"target", targetInstance ? targetInstance->name : ""},
        };
    }

    void CustomLoad(json j) override {
        radius = j["radius"];
        pitch = j["pitch"];
        yaw = j["yaw"];
        pitch_prev = j["pitch_prev"];
        yaw_prev = j["yaw_prev"];
        targetInstance = j["target"] != "" ? instanceStore.at(j["target"]) : NULL;
    }

    json CustomInterpolator(json from, json to, float progress) override {
        return {
            {"radius", Blib::lerp(float(from["radius"]), float(to["radius"]), progress)},
            {"pitch", Blib::lerp(float(from["pitch"]), float(to["pitch"]), progress)},
            {"yaw", Blib::lerp(float(from["yaw"]), float(to["yaw"]), progress)},
            {"pitch_prev", Blib::lerp(float(from["pitch_prev"]), float(to["pitch_prev"]), progress)},
            {"yaw_prev", Blib::lerp(float(from["yaw_prev"]), float(to["yaw_prev"]), progress)},
            {"target", from["target"]},
        };
    }
};
extern Camera camera;
}
