#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>

namespace Blib {
class Mouse {
    double mouseDownX, mouseDownY;
    std::map<int, bool> buttonPressed;
    std::map<int, glm::vec2> buttonDelta;

public:
    bool getButtonPressed(int GLFW_MOUSE_BUTTON) { return buttonPressed[GLFW_MOUSE_BUTTON]; }
    glm::vec2 getDeltaButton(int GLFW_MOUSE_BUTTON) { return buttonDelta[GLFW_MOUSE_BUTTON]; }

    void Update(GLFWwindow *window);
};
extern Mouse mouse;
}
