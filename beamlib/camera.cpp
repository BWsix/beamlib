#include "camera.h"
#include "mouse.h"

namespace Blib {
Camera camera(NULL, "CAMERA");
}

void Blib::Camera::Update() {
    if (Blib::mouse.getRightButtonPressed()) {
        yaw += sensitivity * Blib::mouse.getDeltaRight().x;
        pitch -= sensitivity * Blib::mouse.getDeltaRight().y;
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    if (isFirstPersonMode()) {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 frontXZ = glm::normalize(glm::vec3(getFront().x, 0, getFront().z));
        glm::vec3 right = glm::normalize(glm::cross(frontXZ, up));

        if (ImGui::IsKeyDown(ImGuiKey_W)) {
            transform.Translate(movementSpeed * frontXZ * Blib::getDeltaTime());
        }
        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            transform.Translate(-movementSpeed * frontXZ * Blib::getDeltaTime());
        }
        if (ImGui::IsKeyDown(ImGuiKey_D)) {
            transform.Translate(movementSpeed * right * Blib::getDeltaTime());
        }
        if (ImGui::IsKeyDown(ImGuiKey_A)) {
            transform.Translate(-movementSpeed * right * Blib::getDeltaTime());
        }
        if (ImGui::IsKeyDown(ImGuiKey_Space)) {
            transform.Translate(movementSpeed * up * Blib::getDeltaTime());
        }
        if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
            transform.Translate(-movementSpeed * up * Blib::getDeltaTime());
        }
    } else {
        if (ImGui::IsKeyDown(ImGuiKey_W)) {
            radius -= movementSpeed * Blib::getDeltaTime();
        }
        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            radius += movementSpeed * Blib::getDeltaTime();
        }
        if (radius < 0.1) {
            radius = 0.1;
        }
    }
}
