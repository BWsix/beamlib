#include "camera.h"
#include "imgui.h"
#include "mouse.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Blib {
Camera camera("CAMERA");
}

void Blib::Camera::Update() {
    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) {
        return;
    }

    if (Blib::mouse.getButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        yaw += sensitivity * Blib::mouse.getDeltaButton(GLFW_MOUSE_BUTTON_MIDDLE).x;
        pitch -= sensitivity * Blib::mouse.getDeltaButton(GLFW_MOUSE_BUTTON_MIDDLE).y;
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
