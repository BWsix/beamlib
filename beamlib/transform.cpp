#include "transform.h"

void Blib::Transform::RenderUI(std::string name) {
    ImGui::DragFloat3(("position##" + name).c_str(), glm::value_ptr(position), 0.05);
    ImGui::DragFloat3(("scale##" + name).c_str(), glm::value_ptr(scale), 0.05);

    glm::vec3 newRotation = getRotationEuler();
    ImGui::DragFloat3(("rotation##" + name).c_str(), glm::value_ptr(newRotation), 0.05);
    glm::vec3 deltaRotation = newRotation - getRotationEuler();
    Rotate(glm::angleAxis(deltaRotation[0], glm::vec3(1, 0, 0)));
    Rotate(glm::angleAxis(deltaRotation[1], glm::vec3(0, 1, 0)));
    Rotate(glm::angleAxis(deltaRotation[2], glm::vec3(0, 0, 1)));

    glm::vec3 newLocalRotation = getLocalRotationEuler();
    ImGui::DragFloat3(("localRotation##" + name).c_str(), glm::value_ptr(newLocalRotation), 0.05);
    glm::vec3 deltaLocalRotation = newLocalRotation - getLocalRotationEuler();
    RotateLocal(glm::angleAxis(deltaLocalRotation[0], glm::vec3(1, 0, 0)));
    RotateLocal(glm::angleAxis(deltaLocalRotation[1], glm::vec3(0, 1, 0)));
    RotateLocal(glm::angleAxis(deltaLocalRotation[2], glm::vec3(0, 0, 1)));
}
