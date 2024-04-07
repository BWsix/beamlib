#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Blib {
class Mouse {
    double mouseDownX, mouseDownY;
    bool rightButtonPrssed = false;
    glm::vec2 deltaRight = {0, 0};

public:
    bool getRightButtonPressed() const { return rightButtonPrssed; }
    glm::vec2 getDeltaRight() const { return deltaRight; }

    void Update(GLFWwindow *window);
};
extern Mouse mouse;
}
